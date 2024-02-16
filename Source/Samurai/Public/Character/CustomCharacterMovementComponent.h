// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums/MovementEnums.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SAMURAI_API UCustomCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_UCLASS_BODY()

	UCustomCharacterMovementComponent();
	
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	virtual void PhysWalking(float deltaTime, int32 Iterations) override;
	virtual float GetMaxAcceleration() const override;
	virtual float GetMaxBrakingDeceleration() const override;

	UPROPERTY()
	uint8 bRequestMovementSettingsUpdate = 1;
	
	UPROPERTY()
	EALS_Gait AllowedGait = EALS_Gait::Walking;

	UPROPERTY(BlueprintReadOnly, Category = "MovementSystem")
	FMovementSettings CurrentMovementSettings;

	void SetMovementSettings(FMovementSettings NewMovementSettings);
	float GetMappedSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Movement Settings")
	void SetAllowedGait(EALS_Gait NewAllowedGait);
};
