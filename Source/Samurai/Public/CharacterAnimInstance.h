// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "HelpfulFunctionLibrary.h"
#include "Animation/AnimInstance.h"
#include "Structs/AnimInstanceEnums.h"
#include "Enums/MovementEnums.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SAMURAI_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void UpdateMovementValues(float UpdateMovementValues);
	void UpdateRotationValues();

	EMovementDirection CalculateMovementDirection() const;
	
	float CalculateWalkRunBlend();
	float CalculateStrideRunBlend();
	float CalculateStandingPlayRate();
	float CalculateCrouchingPlayRate();
	
private:
	UPROPERTY(BlueprintReadOnly, Category = "Character Information", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<ABaseCharacter> Character = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "Anim Graph", meta = (AllowPrivateAccess = "True"))
	FVector RelativeAccelerationAmountC = FVector(0, 0, 0);

	UPROPERTY(BlueprintReadWrite, Category = "Anim Graph - Grounded", meta = (AllowPrivateAccess = "True"))
	float DiagonalScaleAmountC = 0.0;

	UPROPERTY(BlueprintReadWrite, Category = "Anim Graph - Grounded", meta = (AllowPrivateAccess = "True"))
	FVelocityBlendCpp VelocityBlend = {};
	
	UPROPERTY(BlueprintReadWrite, Category = "Anim Graph - Grounded", meta = (AllowPrivateAccess = "True"))
	FLeanAmountC LeanAmountC = {};

	UPROPERTY(BlueprintReadWrite, Category = "Anim Graph - Grounded", meta = (AllowPrivateAccess = "True"))
	float FYawC = 0.0;

	UPROPERTY(BlueprintReadWrite, Category = "Anim Graph - Grounded", meta = (AllowPrivateAccess = "True"))
	float BYawC = 0.0;

	UPROPERTY(BlueprintReadWrite, Category = "Anim Graph - Grounded", meta = (AllowPrivateAccess = "True"))
	float LYawC = 0.0;

	UPROPERTY(BlueprintReadWrite, Category = "Anim Graph - Grounded", meta = (AllowPrivateAccess = "True"))
	float RYawC = 0.0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Main Configuration", meta = (AllowPrivateAccess = "True"))
	FALSAnimConfiguration Config;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Grounded", meta = (AllowPrivateAccess = "True"))
	float WalkRunBlend = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Grounded", meta = (AllowPrivateAccess = "True"))
	float StandingPlayRate = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Grounded", meta = (AllowPrivateAccess = "True"))
	float CrouchingPlayRate = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Grounded", meta = (AllowPrivateAccess = "True"))
	float StrideBlend = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Grounded", meta = (AllowPrivateAccess = "True"))
	bool bShouldMove = false;

	/** Blend Curves */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Blend Curves", meta = (AllowPrivateAccess = "True"))
	TObjectPtr<UCurveFloat> DiagonalScaleAmountCurve = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Blend Curves", meta = (AllowPrivateAccess = "True"))
	TObjectPtr<UCurveFloat> StrideBlend_N_Walk = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Blend Curves", meta = (AllowPrivateAccess = "True"))
	TObjectPtr<UCurveFloat> StrideBlend_N_Run = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Blend Curves", meta = (AllowPrivateAccess = "True"))
	TObjectPtr<UCurveFloat> StrideBlend_C_Walk = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Blend Curves", meta = (AllowPrivateAccess = "True"))
	TObjectPtr<UCurveVector> YawOffset_FB = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Blend Curves", meta = (AllowPrivateAccess = "True"))
	TObjectPtr<UCurveVector> YawOffset_LR = nullptr;

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	FALSAnimCharacterInformation CharacterInformation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	FALSMovementState MovementState = EALS_MovementState::None;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	EMovementDirection MovementDirection = EMovementDirection::Forward;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	FALSGait Gait = EALS_Gait::Walking;
};
