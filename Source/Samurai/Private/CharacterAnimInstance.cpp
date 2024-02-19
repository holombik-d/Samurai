// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"

#include "BaseCharacter.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<AMainCharacter>(TryGetPawnOwner());
}

void UCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(Character == nullptr || DeltaSeconds == 0.0f)
	{
		return;
	}

	CharacterInformation.MovementInputAmount = Character->GetMovementInputAmount();
	CharacterInformation.bHasMovementInput = Character->GatHasMovementInput();
	CharacterInformation.bIsMoving = Character->GetIsMoving();
	CharacterInformation.Acceleration = Character->GetAcceleration();
	CharacterInformation.Speed = Character->GetSpeed();
	CharacterInformation.AimYawRate = Character->GetAimYawRate();
	CharacterInformation.AimingRotation = Character->GetAimRotation();
	CharacterInformation.Velocity = Character->GetCharacterMovement()->Velocity;
	CharacterInformation.MovementInput = Character->GetMovementInput();
	CharacterInformation.CharacterActorRotation = Character->GetRotation();
	CharacterInformation.PreviousMovementState = Character->GetPrevMovementState();
	
	MovementState = Character->GetMovementState();
	Gait = Character->GetGait();

	if(MovementState.Grounded())
	{
		UpdateMovementValues(DeltaSeconds);
		UpdateRotationValues();
	}
}

void UCharacterAnimInstance::UpdateMovementValues(float DeltaSeconds)
{
	const FVelocityBlendCpp& TargetBlend = UHelpfulFunctionLibrary::CalcVelocityBlendFast(this, CharacterInformation.Velocity);
	
	VelocityBlend.F = FMath::FInterpTo(VelocityBlend.F, TargetBlend.F, DeltaSeconds, Config.VelocityBlendInterpSpeed);
	VelocityBlend.B = FMath::FInterpTo(VelocityBlend.B, TargetBlend.B, DeltaSeconds, Config.VelocityBlendInterpSpeed);
	VelocityBlend.L = FMath::FInterpTo(VelocityBlend.L, TargetBlend.L, DeltaSeconds, Config.VelocityBlendInterpSpeed);
	VelocityBlend.R = FMath::FInterpTo(VelocityBlend.R, TargetBlend.R, DeltaSeconds, Config.VelocityBlendInterpSpeed);
	
	DiagonalScaleAmountC = UHelpfulFunctionLibrary::CalculateDiagonalScale(DiagonalScaleAmountCurve, VelocityBlend);
	RelativeAccelerationAmountC = UHelpfulFunctionLibrary::CalculateRelativeAcceleration(this, Character->GetCharacterMovement(),
															CharacterInformation.CharacterActorRotation, CharacterInformation.Acceleration, CharacterInformation.Velocity);

	LeanAmountC.LR = FMath::FInterpTo(LeanAmountC.LR, RelativeAccelerationAmountC.Y, DeltaSeconds, Config.GroundedLeanInterpSpeed);
	LeanAmountC.FB = FMath::FInterpTo(LeanAmountC.FB, RelativeAccelerationAmountC.X, DeltaSeconds, Config.GroundedLeanInterpSpeed);

	WalkRunBlend = CalculateWalkRunBlend();
	StrideBlend = CalculateStrideRunBlend();

	StandingPlayRate = CalculateStandingPlayRate();
	CrouchingPlayRate = CalculateCrouchingPlayRate();
}

void UCharacterAnimInstance::UpdateRotationValues()
{
	MovementDirection = CalculateMovementDirection();
	
	FRotator Delta = CharacterInformation.Velocity.ToOrientationRotator() - CharacterInformation.AimingRotation;
	Delta.Normalize();
	
	const FVector& FBOffset = YawOffset_FB->GetVectorValue(Delta.Yaw);
	FYawC = FBOffset.X;
	BYawC = FBOffset.Y;

	const FVector& LROffset = YawOffset_LR->GetVectorValue(Delta.Yaw);
	LYawC = LROffset.X;
	RYawC = LROffset.Y;
}

EMovementDirection UCharacterAnimInstance::CalculateMovementDirection() const
{
	if(Gait.IsSprinting())
	{
		return EMovementDirection::Forward;
	}

	FRotator Delta = CharacterInformation.Velocity.ToOrientationRotator() - CharacterInformation.AimingRotation;
	Delta.Normalize();
	return UHelpfulFunctionLibrary::CalculateQuadrant(MovementDirection, 70.f, -70.f, 110.f, -110.f, 5.0f, Delta.Yaw);
}

float UCharacterAnimInstance::CalculateWalkRunBlend()
{
	return Gait.IsWalking() ? 0.0f : 1.0f;
}

float UCharacterAnimInstance::CalculateStrideRunBlend()
{
	const float CurveTime = CharacterInformation.Speed / GetOwningComponent()->GetComponentScale().Z;
	const float ClampedGait = UHelpfulFunctionLibrary::GetClampedCurveValue(this,"Weight_Gait", -1.0, 0.0f, 1.f);
	const float LerpedStrideBlend = FMath::Lerp(StrideBlend_N_Walk->GetFloatValue(CurveTime), StrideBlend_N_Run->GetFloatValue(CurveTime), ClampedGait);

	return FMath::Lerp(LerpedStrideBlend, StrideBlend_C_Walk->GetFloatValue(CharacterInformation.Speed), GetCurveValue("BasePose_CLF"));
}

float UCharacterAnimInstance::CalculateStandingPlayRate()
{
	const float LerpedSpeed = FMath::Lerp(CharacterInformation.Speed / Config.AnimatedWalkSpeed,
											CharacterInformation.Speed / Config.AnimatedRunSpeed,
											UHelpfulFunctionLibrary::GetClampedCurveValue(this,"Weight_Gait", -1.0f, 0.0f, 1.0f));

	const float SprintAffectSpeed = FMath::Lerp(LerpedSpeed, CharacterInformation.Speed / Config.AnimatedSprintSpeed,
												UHelpfulFunctionLibrary::GetClampedCurveValue(this, "Weight_Gait", -2.0f, 0.0f, 1.0f));

	return FMath::Clamp((SprintAffectSpeed / StrideBlend) / GetOwningComponent()->GetComponentScale().Z, 0.0f, 3.0f);
}

float UCharacterAnimInstance::CalculateCrouchingPlayRate()
{
	return FMath::Clamp(CharacterInformation.Speed / Config.AnimatedCrouchSpeed / StrideBlend / GetOwningComponent()->GetComponentScale().Z, 0.0f, 2.0f);
}
