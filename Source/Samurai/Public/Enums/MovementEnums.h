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

USTRUCT(Blueprintable)
struct FALSMovementState
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Movement System")
	EALS_MovementState State = EALS_MovementState::None;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Movement System")
	bool _None = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Movement System")
	bool _Grounded = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Movement System")
	bool _isInAir = false;

public:
	FALSMovementState() {};
	FALSMovementState(const EALS_MovementState InitialState) { *this = InitialState; }

	const bool& Grounded() const { return _Grounded; }

	operator EALS_MovementState() const { return State; }
	void operator=(const EALS_MovementState NewState)
	{
		State = NewState;
		_None = State == EALS_MovementState::Grounded;
		_Grounded = State == EALS_MovementState::Grounded;
		_isInAir = State == EALS_MovementState::InAir;
	}
};

USTRUCT(BlueprintType)
struct FALSGait
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ALS|Movement System")
	EALS_Gait Gait = EALS_Gait::Walking;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ALS|Movement System")
	bool _Walking = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ALS|Movement System")
	bool _Running = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ALS|Movement System")
	bool _Sprinting = false;

public:
	FALSGait() {};
	FALSGait(const EALS_Gait InitialGait) { *this = InitialGait; }

	const bool& IsWalking() const { return _Walking; }
	const bool& IsRunning() const { return _Running; }
	const bool& IsSprinting() const { return  _Sprinting; }

	operator FALSGait() const { return Gait; }

	void operator=(const EALS_Gait NewGait)
	{
		Gait = NewGait;
		_Walking = Gait == EALS_Gait::Walking;
		_Running = Gait == EALS_Gait::Running;
		_Sprinting = Gait == EALS_Gait::Sprinting;
	}
};