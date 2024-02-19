// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CustomCharacterMovementComponent.h"

UCustomCharacterMovementComponent::UCustomCharacterMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UCustomCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation,
                                                          const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	if(CharacterOwner == nullptr)
	{
		return;
	}

	if(bRequestMovementSettingsUpdate)
	{
		const float UpdateMaxSpeed = CurrentMovementSettings.GetSpeedForGait(AllowedGait);
		MaxWalkSpeed = UpdateMaxSpeed;
		MaxWalkSpeedCrouched = UpdateMaxSpeed;

		bRequestMovementSettingsUpdate = false;
	}
}

void UCustomCharacterMovementComponent::PhysWalking(float deltaTime, int32 Iterations)
{
	if(CurrentMovementSettings.MovementCurve)
	{
		GroundFriction = CurrentMovementSettings.MovementCurve->GetVectorValue(GetMappedSpeed()).Z;
	}
	
	Super::PhysWalking(deltaTime, Iterations);
}

float UCustomCharacterMovementComponent::GetMaxAcceleration() const
{
	if(IsMovingOnGround() || CurrentMovementSettings.MovementCurve == nullptr)
	{
		return Super::GetMaxAcceleration();
	}
	return CurrentMovementSettings.MovementCurve->GetVectorValue(GetMappedSpeed()).X;
}

float UCustomCharacterMovementComponent::GetMaxBrakingDeceleration() const
{
	if (!IsMovingOnGround() || !CurrentMovementSettings.MovementCurve)
	{
		return Super::GetMaxBrakingDeceleration();
	}
	
	return CurrentMovementSettings.MovementCurve->GetVectorValue(GetMappedSpeed()).Y;
}

void UCustomCharacterMovementComponent::SetMovementSettings(FMovementSettings NewMovementSettings)
{
	CurrentMovementSettings = NewMovementSettings;
	bRequestMovementSettingsUpdate = true;
}

float UCustomCharacterMovementComponent::GetMappedSpeed() const
{
	const float Speed = Velocity.Size2D();
	const float LocWalkSpeed = CurrentMovementSettings.WalkSpeed;
	const float LocRunSpeed = CurrentMovementSettings.RunSpeed;
	const float LocSprintSpeed = CurrentMovementSettings.SprintSpeed;

	if(Speed > LocRunSpeed)
	{
		return FMath::GetMappedRangeValueClamped<float, float>({LocRunSpeed, LocSprintSpeed}, {2.0f, 3.0f}, Speed);
	}

	if(Speed > LocWalkSpeed)
	{
		return FMath::GetMappedRangeValueClamped<float, float>({LocWalkSpeed, LocRunSpeed}, {1.0f, 2.0f}, Speed);
	}

	return FMath::GetMappedRangeValueClamped<float, float>({0.0f, LocWalkSpeed}, {0.0f, 1.0f}, Speed);
}

void UCustomCharacterMovementComponent::SetAllowedGait(EALS_Gait NewAllowedGait)
{
	if(AllowedGait != NewAllowedGait)
	{
		if(PawnOwner->IsLocallyControlled())
		{
			AllowedGait = NewAllowedGait;
			bRequestMovementSettingsUpdate = true;
			return;
		}
	}
}
