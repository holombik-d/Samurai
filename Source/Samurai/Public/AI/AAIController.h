// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AAIController.generated.h"

/**
 * 
 */
UCLASS()
class SAMURAI_API AMyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMyAIController();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree = nullptr;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual FVector GetFocalPointOnActor(const AActor* Actor) const override;
};
