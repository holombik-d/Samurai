// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Character\CustomPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void AMyPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	_PossesedCharacter = Cast<AMainCharacter>(InPawn);

	if(_PossesedCharacter != nullptr)
	{
		SetupInputs();
	}
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if(EnhancedInputComponent)
	{
		EnhancedInputComponent->ClearActionBindings();
		EnhancedInputComponent->ClearActionValueBindings();

		BindActions(_DefaultMappingContext);
	}
}

void AMyPlayerController::SetupInputs()
{
	if(_PossesedCharacter)
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			FModifyContextOptions ContextOptions;
			ContextOptions.bForceImmediately = true;
			Subsystem->AddMappingContext(_DefaultMappingContext, 1, ContextOptions);
		}
	}
}

void AMyPlayerController::BindActions(const UInputMappingContext* Context)
{
	if(Context)
	{
		const TArray<FEnhancedActionKeyMapping>& Mappings = Context->GetMappings();
		UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
		if(EnhancedInput)
		{
			TSet<const UInputAction*> UniqueActions;
			for(const FEnhancedActionKeyMapping& KeyMapping : Mappings)
			{
				UniqueActions.Add(KeyMapping.Action);
			}

			for(const UInputAction* UniqueAction : UniqueActions)
			{
				EnhancedInput->BindAction(UniqueAction, ETriggerEvent::Triggered, Cast<UObject>(this), UniqueAction->GetFName());
			}
		}
	}
}

void AMyPlayerController::ForwardMovementAction(const FInputActionValue& Value)
{
	if(_PossesedCharacter)
	{
		_PossesedCharacter->ForwardMovementAction(Value.GetMagnitude());
	}
}

void AMyPlayerController::RightMovementAction(const FInputActionValue& Value)
{
	if(_PossesedCharacter)
	{
		_PossesedCharacter->RightMovementAction(Value.GetMagnitude());
	}
}

void AMyPlayerController::JumpAction(const FInputActionValue& Value)
{
	if(_PossesedCharacter)
	{
		_PossesedCharacter->JumpAction(Value.Get<bool>());
	}
}

void AMyPlayerController::SprintAction(const FInputActionValue& Value)
{
	if(_PossesedCharacter)
	{
		_PossesedCharacter->SprintAction(Value.Get<bool>());
	}
}

void AMyPlayerController::WalkAction(const FInputActionValue& Value)
{
	if(_PossesedCharacter)
	{
		_PossesedCharacter->WalkAction();
	}
}

void AMyPlayerController::StanceAction(const FInputActionValue& Value)
{
	if(_PossesedCharacter && Value.Get<bool>())
	{
		_PossesedCharacter->StanceAction();
	}
}

void AMyPlayerController::CameraUpAction(const FInputActionValue& Value)
{
	if(_PossesedCharacter)
	{
		_PossesedCharacter->CameraUpAction(Value.GetMagnitude());
	}
}

void AMyPlayerController::CameraRightAction(const FInputActionValue& Value)
{
	if(_PossesedCharacter)
	{
		_PossesedCharacter->CameraRightAction(Value.GetMagnitude());
	}
}
