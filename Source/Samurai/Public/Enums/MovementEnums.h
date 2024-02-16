#pragma once
#include "Curves/CurveVector.h"
#include "Engine/DataTable.h"
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

UENUM(BlueprintType) // Optional for Blueprint accessibility
enum class EMovementDirection : uint8
{
	Forward,
	Right,
	Left,
	Backward
};

UENUM(BlueprintType, meta = (ScriptName="RotationMode"))
enum class ERotationMode : uint8
{
	VelocityDirection,
	LookingDirection,
	Aiming
};

USTRUCT(BlueprintType)
struct FMovementSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	float WalkSpeed = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	float RunSpeed = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	float SprintSpeed = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	TObjectPtr<UCurveVector> MovementCurve = nullptr;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	TObjectPtr<UCurveFloat> RotationRateCurve = nullptr;

	float GetSpeedForGait(const EALS_Gait Gait) const
	{
		switch (Gait)
		{
		case EALS_Gait::Running:
			return RunSpeed;
		case EALS_Gait::Sprinting:
			return SprintSpeed;
		case EALS_Gait::Walking:
			return WalkSpeed;
		default:
			return RunSpeed;
		}
	}
};

USTRUCT(BlueprintType)
struct FMovementStanceSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	FMovementSettings Standing;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	FMovementSettings Crouching;
};

USTRUCT(BlueprintType)
struct FMovementStateSettings : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	FMovementStanceSettings VelocityDirection;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	FMovementStanceSettings LookingDirection;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	FMovementStanceSettings Aiming;
};
