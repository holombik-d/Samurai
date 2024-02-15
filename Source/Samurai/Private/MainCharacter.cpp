// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->AddTickPrerequisiteActor(this);

	if(Stance == EALS_Stance::Crouching)
	{
		UnCrouch();
	}
	else if(Stance == EALS_Stance::Crouching)
	{
		Crouch();
	}

	TargetRotation = GetActorRotation();
	LastVelocityRotation = TargetRotation;
	LastMovementInputRotation = TargetRotation;
}

void AMainCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	
	SetStance(EALS_Stance::Crouching);
}

void AMainCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	SetStance(EALS_Stance::Standing);
}

void AMainCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if(GetCharacterMovement()->MovementMode == MOVE_Walking || GetCharacterMovement()->MovementMode == MOVE_NavWalking)
	{
		SetMovementState(EALS_MovementState::Grounded);
	}
	else if(GetCharacterMovement()->MovementMode == MOVE_Falling)
	{
		SetMovementState(EALS_MovementState::InAir);
	}
}


// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SetEssentialValues(DeltaTime);
	CacheValues();
}

void AMainCharacter::SetDesiredGait(EALS_Gait NewGait)
{
	DesiredGait = NewGait;
}

void AMainCharacter::SetDesireStance(EALS_Stance NewStance)
{
	DesiredStance = NewStance;
}

void AMainCharacter::SetStance(EALS_Stance NewStance, bool bForce)
{
	if(bForce || Stance != NewStance)
	{
		const EALS_Stance PreviousStance = Stance;
		Stance = NewStance;
		//OnStanceChanged(PreviousStance)
	}
}

void AMainCharacter::SetMovementState(EALS_MovementState NewMovementState)
{
	MovementState = NewMovementState;
}

EALS_Stance AMainCharacter::GetStance()
{
	return Stance;
}

void AMainCharacter::SetEssentialValues(float DeltaTime)
{
	CurrentAcceleration = GetCharacterMovement()->GetCurrentAcceleration();
	ControlRotation = GetControlRotation();
	EasedMaxAcceleration = GetCharacterMovement()->GetMaxAcceleration();

	AimingRotation = FMath::RInterpTo(AimingRotation, ControlRotation, DeltaTime, 30);

	const FVector CurrentVelocity = GetVelocity();

	const FVector NewAcceleration = ((CurrentAcceleration - PreviousVelocity) / DeltaTime);
	Acceleration = NewAcceleration.IsNearlyZero() || IsLocallyControlled() ? NewAcceleration : Acceleration / 2;

	Speed = CurrentVelocity.Size2D();
	bIsMoving = Speed > 1.0f;

	if(bIsMoving)
	{
		LastVelocityRotation = CurrentVelocity.ToOrientationRotator();
	}

	MovementInputAmount = CurrentAcceleration.Size() / EasedMaxAcceleration;
	bHasMovementInput = MovementInputAmount > 0.0f;
	if(bHasMovementInput)
	{
		LastMovementInputRotation = CurrentAcceleration.ToOrientationRotator();
	}

	AimYawRate = FMath::Abs((AimingRotation.Yaw - PreviousAimYaw) / DeltaTime);
}

void AMainCharacter::CacheValues()
{
	PreviousVelocity = GetVelocity();
	PreviousAimYaw = AimingRotation.Yaw;
}

void AMainCharacter::ForwardMovementAction_Implementation(float Value)
{
	const FRotator DirectionRotator(0.0f, AimingRotation.Yaw, 0.0f);
	AddMovementInput(UKismetMathLibrary::GetForwardVector(DirectionRotator), Value);
}

void AMainCharacter::RightMovementAction_Implementation(float Value)
{
	const FRotator DirectionRotator(0.0f, AimingRotation.Yaw, 0.0f);
	AddMovementInput(UKismetMathLibrary::GetRightVector(DirectionRotator), Value);
}

void AMainCharacter::CameraUpAction_Implementation(float Value)
{
	AddControllerPitchInput(LookUpDownRate * Value);
}

void AMainCharacter::CameraRightAction_Implementation(float Value)
{
	AddControllerYawInput(LookLeftRightRate * Value);
}

void AMainCharacter::JumpAction_Implementation(bool bValue)
{
	if(bValue)
	{
		Jump();
	}
	else
	{
		StopJumping();
	}
}

void AMainCharacter::SprintAction_Implementation(bool bValue)
{
	if(bValue)
	{
		SetDesiredGait(EALS_Gait::Sprinting);
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "EALS_Gait::Sprinting");
	}
	else
	{
		SetDesiredGait(EALS_Gait::Running);
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "EALS_Gait::Running");
	}
}

void AMainCharacter::StanceAction_Implementation()
{
	UWorld* World = GetWorld();
	check(World);

	const float PreviousStanceInputTime = LastStanceInputTime;
	LastStanceInputTime = World->GetTimeSeconds();
	if(LastStanceInputTime - PreviousStanceInputTime <= 0.3f)
	{
		if(Stance != EALS_Stance::Standing)
		{
			SetDesireStance(EALS_Stance::Crouching);
		}
		else if(Stance != EALS_Stance::Crouching)
		{
			SetDesireStance(EALS_Stance::Standing);
		}
		return;
	}

	if(MovementState == EALS_MovementState::Grounded)
	{
		if(Stance == EALS_Stance::Standing)
		{
			SetDesireStance(EALS_Stance::Crouching);
			Crouch();
		}
		else if(Stance == EALS_Stance::Crouching)
		{
			SetDesireStance(EALS_Stance::Standing);
			UnCrouch();
		}
	}
}

void AMainCharacter::WalkAction_Implementation()
{
	if(DesiredGait == EALS_Gait::Walking)
	{
		SetDesiredGait(EALS_Gait::Running);
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "SetDesiredGait(EALS_Gait::Running);");
	}
	else if(DesiredGait == EALS_Gait::Running)
	{
		SetDesiredGait(EALS_Gait::Walking);
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "SetDesiredGait(EALS_Gait::Walking);");
	}
}
