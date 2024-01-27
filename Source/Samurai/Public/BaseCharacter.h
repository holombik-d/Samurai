// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "Enums/MovementEnums.h"
#include "BaseCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SAMURAI_API ABaseCharacter : public AMainCharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaSeconds) override;

private:
	UFUNCTION(BlueprintCallable)
	void LookUpAndDown(float Axis);

	UFUNCTION(BlueprintCallable)
	void LookLeftAndRight(float Axis);

	UFUNCTION(BlueprintCallable)
	void SetEssentialValues();

	FVector CalculateAcceleration() const;
	float GetCharacterSpeedIfItMove() const;

	UFUNCTION(BlueprintPure)
	FTwoVectors GetControlForwardAndRightVector();
	
	UFUNCTION(BlueprintCallable)
	void CreateMovingInputWithInterp(FVector WorldMovementDirectionTarget);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess))
	class USpringArmComponent* CameraBoomComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess))
	class UCameraComponent* CameraComponent;

	UPROPERTY(BlueprintReadWrite)
	EMovementState MovementState = EMovementState::None;

	UPROPERTY(BlueprintReadWrite)
	EMovementState PrevMovementState = EMovementState::None;

	UPROPERTY(BlueprintReadWrite)
	EStance Stance = EStance::Standing;

	UPROPERTY(BlueprintReadWrite)
	EStance PreviousStance = EStance::Standing;

	UPROPERTY(BlueprintReadWrite)
	EStance NewStance = EStance::Standing;
	
	UPROPERTY(BlueprintReadWrite)
	EStance DesiredStance = EStance::Standing;
	
	UPROPERTY(BlueprintReadWrite)
	EPMovementMode MovementMode = EPMovementMode::Default;

	UPROPERTY(BlueprintReadWrite)
	EGate DesiredGate = EGate::Running;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	float LookUpAndDownRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	float LookLefAndRightRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	float MoveForwardAndBackwardScale;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	float MoveLeftAndRightScale;

	UPROPERTY(EditAnywhere,BlueprintReadWrite);
	bool WalkActionPressed;
};
