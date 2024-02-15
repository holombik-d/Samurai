// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->AddTickPrerequisiteActor(this);

	TargetRotation = GetActorRotation();
	LastVelocityRotation = TargetRotation;
	LastMovementInputRotation = TargetRotation;
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SetEssentialValues(DeltaTime);
	CacheValues();
}

void AMainCharacter::SetEssentialValues(float DeltaTime)
{
	CurrentAcceleration = GetCharacterMovement()->GetCurrentAcceleration();
	ControlRotation = GetControlRotation();
	EasedMaxAcceleration = GetCharacterMovement()->GetMaxAcceleration();

	AimingRotation = FMath::RInterpTo(AimingRotation, ControlRotation, DeltaTime, 30);

	const FVector CurrentVelocity = GetVelocity();

	const FVector NewAcceleration = ((CurrentAcceleration - PreviousVelocity) / DeltaTime);
	Acceleration = NewAcceleration.IsNearlyZero() || IsLocallyControlled() ? NewAcceleration : Acceleration / 2;

	Speed = CurrentVelocity.Size2D();
	bIsMoving = Speed > 1.0f;

	if(bIsMoving)
	{
		LastVelocityRotation = CurrentVelocity.ToOrientationRotator();
	}

	MovementInputAmount = CurrentAcceleration.Size() / EasedMaxAcceleration;
	bHasMovementInput = MovementInputAmount > 0.0f;
	if(bHasMovementInput)
	{
		LastMovementInputRotation = CurrentAcceleration.ToOrientationRotator();
	}

	AimYawRate = FMath::Abs((AimingRotation.Yaw - PreviousAimYaw) / DeltaTime);
}

void AMainCharacter::CacheValues()
{
	PreviousVelocity = GetVelocity();
	PreviousAimYaw = AimingRotation.Yaw;
}

void AMainCharacter::ForwardMovementAction_Implementation(float Value)
{
	const FRotator DirectionRotator(0.0f, AimingRotation.Yaw, 0.0f);
	AddMovementInput(UKismetMathLibrary::GetForwardVector(DirectionRotator), Value);
}

void AMainCharacter::RightMovementAction_Implementation(float Value)
{
	const FRotator DirectionRotator(0.0f, AimingRotation.Yaw, 0.0f);
	AddMovementInput(UKismetMathLibrary::GetRightVector(DirectionRotator), Value);
}

void AMainCharacter::CameraUpAction_Implementation(float Value)
{
	AddControllerPitchInput(LookUpDownRate * Value);
}

void AMainCharacter::CameraRightAction_Implementation(float Value)
{
	AddControllerYawInput(LookLeftRightRate * Value);
}

void AMainCharacter::JumpAction_Implementation(bool bValue)
{
	if(bValue)
	{
		Jump();
	}
	else
	{
		StopJumping();
	}
}

void AMainCharacter::SprintAction_Implementation(bool bValue)
{
}

void AMainCharacter::StanceAction_Implementation()
{
}

void AMainCharacter::WalkAction_Implementation()
{
}
