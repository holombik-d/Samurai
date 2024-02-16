// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMainCharacter::AMainCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->AddTickPrerequisiteActor(this);
	
	SetMovementModel();

	ForceUpdateCharacterState();
	
	if(Stance == EALS_Stance::Crouching)
	{
		UnCrouch();
	}
	else if(Stance == EALS_Stance::Crouching)
	{
		Crouch();
	}

	TargetRotation = GetActorRotation();
	LastVelocityRotation = TargetRotation;
	LastMovementInputRotation = TargetRotation;

	CustomCharacterMovementComponent->SetMovementSettings(GetTargetMovementSettings());
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SetEssentialValues(DeltaTime);

	if(MovementState == EALS_MovementState::Grounded)
	{
		UpdateCharacterMovement();
	}
	
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

void AMainCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CustomCharacterMovementComponent = Cast<UCustomCharacterMovementComponent>(Super::GetMovementComponent());
}

EALS_Gait AMainCharacter::GetAllowedGait() const
{
	if(Stance == EALS_Stance::Standing)
	{
		if(RotationMode != ERotationMode::Aiming)
		{
			if(DesiredGait == EALS_Gait::Sprinting)
			{
				return CanSprint() ? EALS_Gait::Sprinting : EALS_Gait::Running;
			}
			return DesiredGait;
		}
	}

	if(DesiredGait == EALS_Gait::Sprinting)
	{
		return EALS_Gait::Running;
	}
	
	return DesiredGait;
}

EALS_Gait AMainCharacter::GetActualGait(EALS_Gait AllowedGait) const
{
	const float LocWalkSpeed = CustomCharacterMovementComponent->CurrentMovementSettings.WalkSpeed;
	const float locRunSpeed = CustomCharacterMovementComponent->CurrentMovementSettings.RunSpeed;

	if(Speed > locRunSpeed + 10.f)
	{
		if(AllowedGait == EALS_Gait::Sprinting)
		{
			return EALS_Gait::Sprinting;
		}
		return EALS_Gait::Running;
	}

	if(Speed >= LocWalkSpeed + 10.f)
	{
		return EALS_Gait::Running;
	}

	return EALS_Gait::Walking;
}

void AMainCharacter::SetGait(EALS_Gait NewGait)
{
	if(Gait != NewGait)
	{
		const EALS_Gait Previous = Gait;
		Gait = NewGait;
	}
}

void AMainCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	
	SetStance(EALS_Stance::Crouching);
}

void AMainCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	SetStance(EALS_Stance::Standing);
}

void AMainCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if(GetCharacterMovement()->MovementMode == MOVE_Walking || GetCharacterMovement()->MovementMode == MOVE_NavWalking)
	{
		SetMovementState(EALS_MovementState::Grounded);
	}
	else if(GetCharacterMovement()->MovementMode == MOVE_Falling)
	{
		SetMovementState(EALS_MovementState::InAir);
	}
}

void AMainCharacter::OnStanceChanged()
{
	CustomCharacterMovementComponent->SetMovementSettings(GetTargetMovementSettings());
}

bool AMainCharacter::CanSprint() const
{
	if(bHasMovementInput == false || RotationMode == ERotationMode::Aiming)
	{
		return false;
	}

	const bool bValidInputAmount = MovementInputAmount > 0.9f;
	if(RotationMode == ERotationMode::LookingDirection)
	{
		const FRotator AccerationRotation = CurrentAcceleration.ToOrientationRotator();
		FRotator Delta = AccerationRotation - AimingRotation;
		Delta.Normalize();
		return bValidInputAmount && FMath::Abs(Delta.Yaw) < 50.0f;
	}
	return false;
}

FMovementSettings AMainCharacter::GetTargetMovementSettings() const
{
	if(RotationMode == ERotationMode::LookingDirection)
	{
		if(Stance == EALS_Stance::Standing)
		{
			return MovementData.LookingDirection.Standing;
		}

		if(Stance == EALS_Stance::Crouching)
		{
			return MovementData.LookingDirection.Crouching;
		}
	}

	//TODO : will be implemented with combat system
	//if(RotationMode == ERotationMode::Aiming)

	return MovementData.VelocityDirection.Standing;
}

void AMainCharacter::SetDesiredGait(EALS_Gait NewGait)
{
	DesiredGait = NewGait;
}

void AMainCharacter::SetDesireStance(EALS_Stance NewStance)
{
	DesiredStance = NewStance;
}

void AMainCharacter::SetStance(EALS_Stance NewStance, bool bForce)
{
	if(bForce || Stance != NewStance)
	{
		Stance = NewStance;
		OnStanceChanged();
	}
}

void AMainCharacter::SetMovementState(EALS_MovementState NewMovementState)
{
	MovementState = NewMovementState;
}

EALS_Stance AMainCharacter::GetStance()
{
	return Stance;
}

void AMainCharacter::CacheValues()
{
	PreviousVelocity = GetVelocity();
	PreviousAimYaw = AimingRotation.Yaw;
}

void AMainCharacter::ForceUpdateCharacterState()
{
	SetGait(DesiredGait);
	SetRotationMode(DesiredRotationMode, true);
}

void AMainCharacter::SetMovementModel()
{
	const FString ContextString = GetFullName();
	FMovementStateSettings* OutRow = MovementModel.DataTable->FindRow<FMovementStateSettings>(MovementModel.RowName, ContextString);
	check(OutRow)
	MovementData = *OutRow;
}

void AMainCharacter::SetRotationMode(const ERotationMode NewRotationMode, bool bForce)
{
	if(bForce || RotationMode != NewRotationMode)
	{
		const ERotationMode Previous = RotationMode;
		RotationMode = NewRotationMode;
		//OnRotationModeChanged(Previous);
		CustomCharacterMovementComponent->SetMovementSettings(GetTargetMovementSettings());
	}
}

void AMainCharacter::SetDesiredRotationMode(ERotationMode NewRotationMode)
{
	DesiredRotationMode = NewRotationMode;
}

void AMainCharacter::UpdateCharacterMovement()
{
	const EALS_Gait AllowedGait = GetAllowedGait();
	const EALS_Gait ActualGait = GetActualGait(AllowedGait);
	if(ActualGait != Gait)
	{
		SetGait(ActualGait);
	}
	CustomCharacterMovementComponent->SetAllowedGait(AllowedGait);
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
	if(bValue)
	{
		SetDesiredGait(EALS_Gait::Sprinting);
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "EALS_Gait::Sprinting");
	}
	else
	{
		SetDesiredGait(EALS_Gait::Running);
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "EALS_Gait::Running");
	}
}

void AMainCharacter::StanceAction_Implementation()
{
	UWorld* World = GetWorld();
	check(World);

	const float PreviousStanceInputTime = LastStanceInputTime;
	LastStanceInputTime = World->GetTimeSeconds();
	if(LastStanceInputTime - PreviousStanceInputTime <= 0.3f)
	{
		if(Stance == EALS_Stance::Standing)
		{
			SetDesireStance(EALS_Stance::Crouching);
		}
		else if(Stance == EALS_Stance::Crouching)
		{
			SetDesireStance(EALS_Stance::Standing);
		}
		return;
	}

	if(MovementState == EALS_MovementState::Grounded)
	{
		if(Stance == EALS_Stance::Standing)
		{
			SetDesireStance(EALS_Stance::Crouching);
			Crouch();
		}
		else if(Stance == EALS_Stance::Crouching)
		{
			SetDesireStance(EALS_Stance::Standing);
			UnCrouch();
		}
	}
}

void AMainCharacter::WalkAction_Implementation()
{
	if(DesiredGait == EALS_Gait::Walking)
	{
		SetDesiredGait(EALS_Gait::Running);
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "SetDesiredGait(EALS_Gait::Running);");
	}
	else if(DesiredGait == EALS_Gait::Running)
	{
		SetDesiredGait(EALS_Gait::Walking);
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "SetDesiredGait(EALS_Gait::Walking);");
	}
}
