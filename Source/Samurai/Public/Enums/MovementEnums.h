#pragma once
#include "MovementEnums.generated.h"

UENUM(BlueprintType) // Optional for Blueprint accessibility
enum class EALS_MovementState : uint8
{
	None,
	Grounded,
	InAir,
	Mantling,
	Ragdoll,
	Crawl,
	Prone
};

UENUM(BlueprintType) // Optional for Blueprint accessibility
enum class EALS_Stance : uint8
{
	Standing,
	Crouching
};

UENUM(BlueprintType) // Optional for Blueprint accessibility
enum class EALSP2_MovementMode : uint8
{
	Default
};

UENUM(BlueprintType) // Optional for Blueprint accessibility
enum class EALS_Gait : uint8
{
	Walking,
	Running,
	Sprinting
};