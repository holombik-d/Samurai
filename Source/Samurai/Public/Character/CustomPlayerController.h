// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "CustomPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SAMURAI_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;

	virtual void SetupInputs();
	virtual void BindActions(const UInputMappingContext* Context);

protected:
	UFUNCTION()
	void ForwardMovementAction(const FInputActionValue& Value);

	UFUNCTION()
	void RightMovementAction(const FInputActionValue& Value);

	UFUNCTION()
	void JumpAction(const FInputActionValue& Value);

	UFUNCTION()
	void SprintAction(const FInputActionValue& Value);

	UFUNCTION()
	void WalkAction(const FInputActionValue& Value);

	UFUNCTION()
	void StanceAction(const FInputActionValue& Value);

	UFUNCTION()
	void CameraUpAction(const FInputActionValue& Value);

	UFUNCTION()
	void CameraRightAction(const FInputActionValue& Value);

private:
	UPROPERTY(BlueprintReadOnly, Category = "Character", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<AMainCharacter> _PossesedCharacter = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Character", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> _DefaultMappingContext = nullptr;
};
