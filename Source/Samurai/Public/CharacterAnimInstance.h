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

	const bool ShouldMoveCheck() const;
	
private:
	UPROPERTY(BlueprintReadOnly, Category = "Character Information", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<ABaseCharacter> Character = nullptr;

	/** Anim Graph - Grounded */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Grounded", meta = (AllowPrivateAccess = "True"))
	FALSAnimGraphGrounded Grounded;

	UPROPERTY(BlueprintReadWrite, Category = "Anim Graph", meta = (AllowPrivateAccess = "True"))
	FVector RelativeAccelerationAmountC = FVector(0, 0, 0);
	
	UPROPERTY(BlueprintReadWrite, Category = "Anim Graph - Grounded", meta = (AllowPrivateAccess = "True"))
	FVelocityBlendCpp VelocityBlend = {};
	
	UPROPERTY(BlueprintReadWrite, Category = "Anim Graph - Grounded", meta = (AllowPrivateAccess = "True"))
	FLeanAmountC LeanAmountC = {};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Main Configuration", meta = (AllowPrivateAccess = "True"))
	FALSAnimConfiguration Config;
	

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

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	EALS_Stance Stance = EALS_Stance::Standing;
};
