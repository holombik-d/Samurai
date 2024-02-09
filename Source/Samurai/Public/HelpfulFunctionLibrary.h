// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HelpfulFunctionLibrary.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FVelocityBlendCpp
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Velocity")
	float F = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Velocity")
	float B = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Velocity")
	float L = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Velocity")
	float R = 0.0;
};

UCLASS()
class SAMURAI_API UHelpfulFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "FunctionsLibrary| Character", meta = (BlueprintThreadSafe, WorldContext = "WorldContextObject", DisplayName = "Update Movement Values (AnimBP)", Keywords = "Character Anim Graph"))
	static void UpdateMovementValues(const UObject* WorldContextObject, const UCharacterMovementComponent* CharMove, FRotator ActorRot, FVelocityBlendCpp VelocityBlend, float DeltaX, float VelocityBlendInterpSpeed, float AnimatedWalkSpeed,
		   float AnimatedRunSpeed, float AnimatedSprintSpeed, float AnimatedCrouchSpeed, float Speed, UCurveFloat* DiagonalScaleAmountCurve, UCurveFloat* StrideBlend_N_Walk, UCurveFloat* StrideBlend_N_Run, 
		   UCurveFloat* StrideBlend_C_Walk, FVector Velocity, FVector Acceleration, FVector2D LeanAmount, float GroundedLeanInterpSpeed, FName WeightCurveName, FName BasePoseCurveName, FVelocityBlendCpp& ReturnVelocityBlend, 
		   float& ReturnDiagonalScale, FVector& ReturnRelativeAcc, FVector2D& ReturnLeanAmount, float& ReturnWalkRunBlend, float& ReturnStrideBlend, float& ReturnStandingPlayRate, float& ReturnCrouchPlayRate);

	UFUNCTION(BlueprintPure, Category = "FunctionsLibrary| Character", meta = (WorldContext = "WorldContextObject", DisplayName = "Calculate Velocity Blend Fast", Keywords = "Character AnimCurve", BlueprintThreadSafe))
	static FVelocityBlendCpp CalcVelocityBlendFast(const UObject* WorldContextObject, FVector Velocity);

	UFUNCTION(BlueprintPure, Category = "JakubW_FunctionsLibrary| Character", meta = (DisplayName = "Calculate Diagonal Scale Amount (Fast)", Keywords = "Movement Animations Character", BlueprintThreadSafe))
	static float CalculateDiagonalScale(UCurveFloat* Curve, FVelocityBlendCpp VelocityBlend);

	UFUNCTION(BlueprintPure, Category = "JakubW_FunctionsLibrary| Character", meta = (BlueprintThreadSafe, WorldContext = "WorldContextObject", DisplayName = "Get Anim Curve Value Clamped", Keywords = "Character AnimCurve"))
	static float GetClampedCurveValue(const UObject* WorldContextObject, FName Curve, float Bias=0, float ClampMin=0, float ClampMax=1);

	UFUNCTION(BlueprintPure, Category = "FunctionsLibrary| Character", meta = (WorldContext = "WorldContextObject", DisplayName = "Calculate Relative Acceleration Amount (Fast)", Keywords = "Movement Animations Character", BlueprintThreadSafe))
	static FVector CalculateRelativeAcceleration(const UObject* WorldContextObject, const UCharacterMovementComponent* CharMove, FRotator CharRot = FRotator(0,0,0), FVector Acceleration = FVector(0,0,0), FVector Velocity = FVector(0,0,0));

	UFUNCTION(BlueprintPure, Category = "FunctionsLibrary| Character", meta = (WorldContext = "WorldContextObject", DisplayName = "Angle In Range", Keywords = "Movement Animations Character", BlueprintThreadSafe))
	static bool AngleInRange(float Angle, float MinAngle, float MaxAngle, float Buffer, bool IncreaseBuffer);
};
