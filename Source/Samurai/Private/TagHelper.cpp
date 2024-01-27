// Fill out your copyright notice in the Description page of Project Settings.


#include "TagHelper.h"

#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"

bool UTagHelper::SwitchOnOwnedTags(const FGameplayTag& NewState)
{
	FGameplayTagContainer OwnedTags;

	const bool LastInGroup = IsTagLeaf(NewState);

	TArray<FGameplayTag> TagArray;
	OwnedTags.GetGameplayTagArray(TagArray);

	TArray<FGameplayTag> TagsToRemove = {};

	for (FGameplayTag& Tag : TagArray)
	{
		int SelectDepth = 0;
		if (LastInGroup == true) SelectDepth = 1;
		const FString a = GetSubTag(Tag, 1);
		const FString b = GetSubTag(NewState, SelectDepth);
		if (a == b)
		{
			TagsToRemove.Add(Tag);
		}
	}
	//Convert To TagContainer
	FGameplayTagContainer TagsContainerToRemove;
	for (const FGameplayTag& Tag : TagsToRemove)
	{
		TagsContainerToRemove.AddTag(Tag);
	}
	
	return true;
}

bool UTagHelper::IsTagLeaf(const FGameplayTag& Tag)
{
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();
	TSharedPtr<FGameplayTagNode> NodePtr = TagsManager.FindTagNode(Tag);
	const FGameplayTagNode* Node = NodePtr.Get();

	if (Node)
	{
		return Node->GetChildTagNodes().Num() == 0;
	}

	return false;
}

FString UTagHelper::GetSubTag(const FGameplayTag& Tag, int32 DesiredDepth)
{
	FString FullTagName = Tag.ToString();
	TArray<FString> SplitTags;
	FullTagName.ParseIntoArray(SplitTags, TEXT("."));

	// If DesiredDepth exceeds the number of segments or is negative, return an empty string.
	if (DesiredDepth < 0 || DesiredDepth >= SplitTags.Num())
	{
		return FString();
	}

	// Find the appropriate depth from the end.
	int32 IndexFromEnd = SplitTags.Num() - DesiredDepth - 1;
	if (IndexFromEnd >= 0 && IndexFromEnd < SplitTags.Num())
	{
		return SplitTags[IndexFromEnd];
	}

	return FString();
}
