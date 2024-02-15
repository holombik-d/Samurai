// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums/MovementEnums.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class SAMURAI_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;

	void SetEssentialValues(float DeltaTime);
	void CacheValues();
	
	UPROPERTY(BlueprintReadOnly, Category = "Essential Information")
	FVector CurrentAcceleration = FVector::ZeroVector;
	FVector Acceleration = FVector::ZeroVector;
	FVector PreviousVelocity = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadOnly, Category = "Essential Information")
	FRotator ControlRotation = FRotator::ZeroRotator;
	FRotator LastMovementInputRotation = FRotator::ZeroRotator;
	FRotator AimingRotation = FRotator::ZeroRotator;
	FRotator LastVelocityRotation = FRotator::ZeroRotator;
	FRotator TargetRotation = FRotator::ZeroRotator;
	
	UPROPERTY(BlueprintReadOnly, Category = "Essential Information")
	float EasedMaxAcceleration = 0.0f;
	float Speed = 0.0f;
	float PreviousAimYaw = 0.0f;
	float MovementInputAmount = 0.0f;

	/** Last time the 'first' crouch/roll button is pressed */
	float LastStanceInputTime = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "ALS|Input", BlueprintReadOnly)
	float LookLeftRightRate = 1.25f;
	UPROPERTY(EditDefaultsOnly, Category = "ALS|Input", BlueprintReadOnly)
	float LookUpDownRate = 1.25f;
	float AimYawRate = 0.0f;
	
	bool bIsMoving = false;
	UPROPERTY(EditDefaultsOnly, Category = "ALS|Input", BlueprintReadOnly)
	bool bHasMovementInput = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Input")
	EALS_Gait DesiredGait = EALS_Gait::Running;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Input")
	EALS_Stance Stance = EALS_Stance::Standing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Input")
	EALS_Stance DesiredStance = EALS_Stance::Standing;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|State Values")
	EALS_MovementState MovementState = EALS_MovementState::None;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|State Values")
	EALS_MovementState PrevMovementState = EALS_MovementState::None;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//*******INPUT*******//
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input")
	void ForwardMovementAction(float Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Input")
	void RightMovementAction(float Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Input")
	void CameraUpAction(float Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Input")
	void CameraRightAction(float Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Input")
	void JumpAction(bool bValue);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Input")
	void SprintAction(bool bValue);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Input")
	void StanceAction();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Input")
	void WalkAction();
	
	UFUNCTION(BlueprintCallable, Category = "Character States")
	void SetDesiredGait(EALS_Gait NewGait);

	UFUNCTION(BlueprintCallable, Category = "Character States")
	void SetDesireStance(EALS_Stance NewStance);

	UFUNCTION(BlueprintCallable, Category = "Character States")
	void SetStance(EALS_Stance NewStance, bool bForce = false);

	UFUNCTION(BlueprintCallable, Category = "Character States")
	void SetMovementState(EALS_MovementState NewMovementState);

	UFUNCTION(BlueprintCallable, Category = "Character States")
	EALS_Stance GetStance();
};
