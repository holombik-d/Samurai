// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"

#include "BaseCharacter.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	auto character = Cast<ABaseCharacter>(TryGetPawnOwner());
	if(IsValid(character))
	{
		Character = character;
	}
}
