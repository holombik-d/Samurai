// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Animation/AnimInstance.h"
#include "Structs\AnimInstanceEnums.h"
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

private:
	UPROPERTY(BlueprintReadOnly,meta=(AllowPrivateAccess = "true"))
	ABaseCharacter* Character = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "Anim Graph", meta = (AllowPrivateAccess = "True"))
	FVector RelativeAccelerationAmountC = FVector(0, 0, 0);

	UPROPERTY(BlueprintReadWrite, Category = "Anim Graph - Grounded", meta = (AllowPrivateAccess = "True"))
	float DiagonalScaleAmountC = 0.0;

	UPROPERTY(BlueprintReadWrite, Category = "Anim Graph - Grounded", meta = (AllowPrivateAccess = "True"))
	FVelocityBlendC VelocityBlend = {};
	
	UPROPERTY(BlueprintReadWrite, Category = "Anim Graph - Grounded", meta = (AllowPrivateAccess = "True"))
	FLeanAmountC LeanAmountC = {};
};
