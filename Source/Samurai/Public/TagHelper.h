// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TagHelper.generated.h"

/**
 * 
 */
UCLASS()
class SAMURAI_API UTagHelper : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static bool SwitchOnOwnedTags(const FGameplayTag& NewState);

	UFUNCTION(BlueprintCallable)
	static bool IsTagLeaf(const FGameplayTag& Tag);
	
	UFUNCTION(BlueprintCallable)
	static FString GetSubTag(const FGameplayTag& Tag, int32 DesiredDepth);
};
