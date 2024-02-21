// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\AI\AAIController.h"

AMyAIController::AMyAIController()
{
	
}

void AMyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(BehaviorTree && InPawn)
	{
		RunBehaviorTree(BehaviorTree);
	}
}

FVector AMyAIController::GetFocalPointOnActor(const AActor* Actor) const
{
	if(Actor == nullptr)
	{
		return FAISystem::InvalidLocation;
	}

	const APawn* FocusPawn = Cast<APawn>(Actor);
	if(FocusPawn)
	{
		return FocusPawn->GetPawnViewLocation();
	}
	return Actor->GetActorLocation();
}
