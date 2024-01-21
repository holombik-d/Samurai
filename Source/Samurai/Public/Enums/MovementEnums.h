#pragma once
#include "MovementEnums.generated.h"

UENUM(BlueprintType) // Optional for Blueprint accessibility
enum class EMovementState : uint8
{
	None,
	Grounded,
	InAir
};

UENUM(BlueprintType) // Optional for Blueprint accessibility
enum class EStance : uint8
{
	//None,
	Standing,
	Crouching
};

UENUM(BlueprintType) // Optional for Blueprint accessibility
enum class EPMovementMode : uint8
{
	Default
};

UENUM(BlueprintType) // Optional for Blueprint accessibility
enum class EGate : uint8
{
	Walking,
	Running,
	Sprinting
};