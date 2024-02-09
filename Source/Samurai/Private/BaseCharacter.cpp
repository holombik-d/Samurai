// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

ABaseCharacter::ABaseCharacter()
{
	CameraBoomComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoomComponent->SetupAttachment(RootComponent);
	CameraBoomComponent->bUsePawnControlRotation = true;

	//Smooth camera movement
	CameraBoomComponent->SocketOffset = {0.0f, 50.0f, 0.0f};
	CameraBoomComponent->bEnableCameraLag = true;
	CameraBoomComponent->bEnableCameraRotationLag = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(CameraBoomComponent, USpringArmComponent::SocketName);

	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ABaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ABaseCharacter::LookUpAndDown(float Axis)
{
	AddControllerPitchInput(Axis * LookUpAndDownRate);
}

void ABaseCharacter::LookLeftAndRight(float Axis)
{
	AddControllerYawInput(Axis * LookLefAndRightRate);
}

void ABaseCharacter::SetEssentialValues()
{
	//Set amount of Acceleration
	AccelerationC = CalculateAcceleration();
	MovementSpeedDifferenceC = GetVelocity().Size2D()-PreviousVelocityC.Size2D();

	//Define is character is moving
	if((SpeedC = GetCharacterSpeedIfItMove()) > 0.0f)
	{
		IsMovingC = true;
		FVector Velocity = GetVelocity();
		LastVelocityRotationC = FRotationMatrix::MakeFromX(Velocity).Rotator();
	}
	else
	{
		IsMovingC = false;
	}
	
	CheckIfHasMovementInput();
	SetAimYawRate();
	CreateMovingInputWithInterp(GetControlForwardAndRightVector().v1);
}

void ABaseCharacter::CacheValues()
{
	PreviousVelocityC = GetVelocity();
	PreviousAimYawC = GetControlRotation().Yaw;
}

void ABaseCharacter::SetAimYawRate()
{
	float DeltaSeconds = GetWorld()->GetDeltaSeconds();
	double RotationDif = GetControlRotation().Yaw - PreviousAimYawC;

	AimYawRateC = UKismetMathLibrary::Abs((RotationDif/ DeltaSeconds));
}

void ABaseCharacter::CheckIfHasMovementInput()
{
	FVector CurrentAcceleration = GetCharacterMovement()->GetCurrentAcceleration();
	float MaxAcceleration = GetCharacterMovement()->GetMaxAcceleration();

	MovementInputAmountC = (CurrentAcceleration.Length() / MaxAcceleration);
	if(MovementInputAmountC > 0.0f)
	{
		HasMovementInputC = true;
		LastMovementInputRotationC = UKismetMathLibrary::MakeRotFromX(CurrentAcceleration);
	}
}

void ABaseCharacter::CreateMovingInputWithInterp(FVector WorldMovementDirectionTarget)
{
	double RelativeAccelerationLength = RelativeAcceleractionC.Length();
	float DeltaSec = GetWorld()->GetDeltaSeconds();

	MovementDirectionInterpC.v1 = FMath::VInterpTo(MovementDirectionInterpC.v1,WorldMovementDirectionTarget, DeltaSec, RelativeAccelerationLength);
}

FTwoVectors ABaseCharacter::GetControlForwardAndRightVector()
{
	FTwoVectors ForwardAndRightVector;
	FRotator Rotation = GetControlRotation();
	ForwardAndRightVector.v1 = UKismetMathLibrary::GetForwardVector(Rotation);
	ForwardAndRightVector.v2 = UKismetMathLibrary::GetRightVector(Rotation);
	return ForwardAndRightVector;
}

FVector ABaseCharacter::CalculateAcceleration() const
{
	return (GetVelocity() - PreviousVelocityC) / GetWorld()->GetDeltaSeconds();
}

float ABaseCharacter::GetCharacterSpeedIfItMove() const
{
	return FVector(GetVelocity().X, GetVelocity().Y,0).Size();
}
