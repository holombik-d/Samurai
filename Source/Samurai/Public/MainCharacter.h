// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	
	UPROPERTY(EditDefaultsOnly, Category = "ALS|Input", BlueprintReadOnly)
	float LookLeftRightRate = 1.25f;
	UPROPERTY(EditDefaultsOnly, Category = "ALS|Input", BlueprintReadOnly)
	float LookUpDownRate = 1.25f;
	float AimYawRate = 0.0f;
	
	bool bIsMoving = false;
	UPROPERTY(EditDefaultsOnly, Category = "ALS|Input", BlueprintReadOnly)
	bool bHasMovementInput = false;
	
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
	
};
