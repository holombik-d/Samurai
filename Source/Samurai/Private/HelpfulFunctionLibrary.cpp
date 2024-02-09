// Fill out your copyright notice in the Description page of Project Settings.


#include "HelpfulFunctionLibrary.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UHelpfulFunctionLibrary::UpdateMovementValues(const UObject* WorldContextObject,
                                                   const UCharacterMovementComponent* CharMove, FRotator ActorRot, FVelocityBlendCpp VelocityBlend, float DeltaX,
                                                   float VelocityBlendInterpSpeed, float AnimatedWalkSpeed, float AnimatedRunSpeed, float AnimatedSprintSpeed,
                                                   float AnimatedCrouchSpeed, float Speed, UCurveFloat* DiagonalScaleAmountCurve, UCurveFloat* StrideBlend_N_Walk,
                                                   UCurveFloat* StrideBlend_N_Run, UCurveFloat* StrideBlend_C_Walk, FVector Velocity, FVector Acceleration,
                                                   FVector2D LeanAmount, float GroundedLeanInterpSpeed, FName WeightCurveName, FName BasePoseCurveName,
                                                   FVelocityBlendCpp& ReturnVelocityBlend, float& ReturnDiagonalScale, FVector& ReturnRelativeAcc,
                                                   FVector2D& ReturnLeanAmount, float& ReturnWalkRunBlend, float& ReturnStrideBlend, float& ReturnStandingPlayRate,
                                                   float& ReturnCrouchPlayRate)
{
	if (!IsValid(CharMove))
	{ return; }

	const UAnimInstance* AnimBP = Cast<UAnimInstance>(WorldContextObject);
	if (!IsValid(AnimBP))
	{ return; }
	// Calculate Smoothed Velocity Blend
	FVelocityBlendCpp TargetVelocityBlend = CalcVelocityBlendFast(WorldContextObject, Velocity);
	ReturnVelocityBlend.F = UKismetMathLibrary::FInterpTo(VelocityBlend.F, TargetVelocityBlend.F, DeltaX, VelocityBlendInterpSpeed);
	ReturnVelocityBlend.B = UKismetMathLibrary::FInterpTo(VelocityBlend.B, TargetVelocityBlend.B, DeltaX, VelocityBlendInterpSpeed);
	ReturnVelocityBlend.L = UKismetMathLibrary::FInterpTo(VelocityBlend.L, TargetVelocityBlend.L, DeltaX, VelocityBlendInterpSpeed);
	ReturnVelocityBlend.R = UKismetMathLibrary::FInterpTo(VelocityBlend.R, TargetVelocityBlend.R, DeltaX, VelocityBlendInterpSpeed);
	// Calculate Diagonal Scale Amount
	ReturnDiagonalScale = CalculateDiagonalScale(DiagonalScaleAmountCurve, ReturnVelocityBlend);
	// Calculate Relative Acceleration Amount
	ReturnRelativeAcc = CalculateRelativeAcceleration(WorldContextObject, CharMove, ActorRot, Acceleration, Velocity);
	// Make Smoothed Lead Amount
	ReturnLeanAmount = UKismetMathLibrary::Vector2DInterpTo(LeanAmount, FVector2D(ReturnRelativeAcc.Y, ReturnRelativeAcc.X), DeltaX, GroundedLeanInterpSpeed);
	// Calculate Stride Blend
	float StrideCurveValue = UKismetMathLibrary::Lerp(StrideBlend_N_Walk->GetFloatValue(Speed), StrideBlend_N_Run->GetFloatValue(Speed), GetClampedCurveValue(AnimBP, WeightCurveName, -1, 0, 1));
	ReturnStrideBlend = UKismetMathLibrary::Lerp(StrideCurveValue, StrideBlend_N_Walk->GetFloatValue(Speed), AnimBP->GetCurveValue(BasePoseCurveName));
	// Calculate Standing Play Rate
	float SPL = UKismetMathLibrary::Lerp(UKismetMathLibrary::SafeDivide(Speed, AnimatedWalkSpeed), UKismetMathLibrary::SafeDivide(Speed, AnimatedRunSpeed), GetClampedCurveValue(AnimBP, WeightCurveName, -1, 0, 1));
	SPL = UKismetMathLibrary::Lerp(SPL, UKismetMathLibrary::SafeDivide(Speed, AnimatedSprintSpeed), GetClampedCurveValue(AnimBP, WeightCurveName, -2, 0, 1));
	SPL = SPL / ReturnStrideBlend;
	SPL = SPL / AnimBP->GetOwningComponent()->K2_GetComponentScale().Z;
	ReturnStandingPlayRate = UKismetMathLibrary::FClamp(SPL, 0, 3);
	// Calculate Crouching PlayRate
	ReturnCrouchPlayRate = UKismetMathLibrary::FClamp(UKismetMathLibrary::SafeDivide(UKismetMathLibrary::SafeDivide(Speed, AnimatedCrouchSpeed), ReturnStrideBlend) 
																									/ AnimBP->GetOwningComponent()->K2_GetComponentScale().Z, 0, 3);

}

FVelocityBlendCpp UHelpfulFunctionLibrary::CalcVelocityBlendFast(const UObject* WorldContextObject, FVector Velocity)
{
	const UAnimInstance* AnimBP = Cast<UAnimInstance>(WorldContextObject);
	if (AnimBP != nullptr)
	{
		FVelocityBlendCpp ReturnStruct;
		UKismetMathLibrary::Vector_Normalize(Velocity, 0.1);
		FVector LocRelativeVelocity = UKismetMathLibrary::Quat_UnrotateVector(UKismetMathLibrary::Conv_RotatorToQuaternion(AnimBP->GetOwningActor()->GetActorRotation()),Velocity);
		float sum = abs(LocRelativeVelocity.X) + abs(LocRelativeVelocity.Y) + abs(LocRelativeVelocity.Z);
		FVector RelativeDirection = LocRelativeVelocity / sum;
		ReturnStruct.F = UKismetMathLibrary::FClamp(RelativeDirection.X, 0, 1);
		ReturnStruct.B = abs(UKismetMathLibrary::FClamp(RelativeDirection.X, -1, 0));
		ReturnStruct.L = abs(UKismetMathLibrary::FClamp(RelativeDirection.Y, -1, 0));
		ReturnStruct.R = UKismetMathLibrary::FClamp(RelativeDirection.Y, 0, 1);
		return ReturnStruct;
	}
	return FVelocityBlendCpp();
}

float UHelpfulFunctionLibrary::CalculateDiagonalScale(UCurveFloat* Curve, FVelocityBlendCpp VelocityBlend)
{
	return Curve->GetFloatValue(abs(VelocityBlend.F+VelocityBlend.B));
}

float UHelpfulFunctionLibrary::GetClampedCurveValue(const UObject* WorldContextObject, FName Curve, float Bias,
	float ClampMin, float ClampMax)
{
	const UAnimInstance* AnimBP = Cast<UAnimInstance>(WorldContextObject);
	if (AnimBP != nullptr)
	{
		return UKismetMathLibrary::FClamp(AnimBP->GetCurveValue(Curve)+Bias,ClampMin,ClampMax);
	}
	return 0.0f;
}

FVector UHelpfulFunctionLibrary::CalculateRelativeAcceleration(const UObject* WorldContextObject,
	const UCharacterMovementComponent* CharMove, FRotator CharRot, FVector Acceleration, FVector Velocity)
{
	FVector ReturnVec = FVector(0,0,0);
	if (IsValid(CharMove) == false) return FVector(999, 0, 0);

	FQuat CharRotQuat = UKismetMathLibrary::Conv_RotatorToQuaternion(CharRot);
	if (UKismetMathLibrary::Dot_VectorVector(Acceleration, Velocity) > 0)
	{
		ReturnVec = UKismetMathLibrary::Vector_ClampSizeMax(Acceleration, CharMove->GetMaxAcceleration());
		ReturnVec = ReturnVec / CharMove->GetMaxAcceleration();
		ReturnVec = UKismetMathLibrary::Quat_UnrotateVector(CharRotQuat, ReturnVec);
	}
	else
	{
		ReturnVec = UKismetMathLibrary::Vector_ClampSizeMax(Acceleration, CharMove->GetMaxBrakingDeceleration());
		ReturnVec = ReturnVec / CharMove->GetMaxBrakingDeceleration();
		ReturnVec = UKismetMathLibrary::Quat_UnrotateVector(CharRotQuat, ReturnVec);
	}
	return ReturnVec;
}

bool UHelpfulFunctionLibrary::AngleInRange(float Angle, float MinAngle, float MaxAngle, float Buffer,
	bool IncreaseBuffer)
{
	if (IncreaseBuffer == true)
	{
		const bool InRange = UKismetMathLibrary::InRange_FloatFloat(Angle, MinAngle - Buffer, MaxAngle + Buffer, true, true);
		return InRange;
	}
	const bool InRange = UKismetMathLibrary::InRange_FloatFloat(Angle, MinAngle = Buffer, MaxAngle - Buffer, true, true);
	return InRange;
}
