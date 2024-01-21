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

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(CameraBoomComponent, USpringArmComponent::SocketName);

	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ABaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	SetEssentialValues();
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

	CreateMovingInputWithInterp(GetControlForwardAndRightVector().v1);
}

void ABaseCharacter::CreateMovingInputWithInterp(FVector WorldMovementDirectionTarget)
{
	double RelativeAccelerationLength = RelativeAcceleractionC.Length();
	float DeltaSec = GetWorld()->GetDeltaSeconds();

	MovementDirectionInterpC.v1 = FMath::VInterpTo(MovementDirectionInterpC.v1,WorldMovementDirectionTarget, DeltaSec, RelativeAccelerationLength);
	UE_LOG(LogTemp, Warning, TEXT("The vector value is: %s"), *MovementDirectionInterpC.v1.ToString());
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
