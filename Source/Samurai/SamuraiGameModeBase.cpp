// Copyright Epic Games, Inc. All Rights Reserved.


#include "SamuraiGameModeBase.h"

#include "BaseCharacter.h"

ASamuraiGameModeBase::ASamuraiGameModeBase()
{
	static ConstructorHelpers::FClassFinder<ABaseCharacter> SamuraiCharacter_BP(TEXT("/Game/Blueprints/CharacterLogic/SamuraiCharacter_BP"));
	if(SamuraiCharacter_BP.Class != nullptr)
	{
		DefaultPawnClass = SamuraiCharacter_BP.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerController_BP(TEXT("/Game/Blueprints/CharacterLogic/CustomPlayerController"));
	if(PlayerController_BP.Class != nullptr)
	{
		PlayerControllerClass = PlayerController_BP.Class;
	}
}
