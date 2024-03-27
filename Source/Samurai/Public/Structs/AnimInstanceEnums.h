#pragma once
#include "Engine/DataTable.h"
#include "Enums/MovementEnums.h"
#include "AnimInstanceEnums.generated.h"

USTRUCT(BlueprintType)
struct FLeanAmountC : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lean")
	float LR = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lean")
	float FB = 0.0;
};

USTRUCT(BlueprintType)
struct FVelocityBlendC : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Forward")
	float F = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Backward")
	float B = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Left")
	float L = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Right")
	float R = 0.0;
};

USTRUCT(BlueprintType)
struct FALSAnimCharacterInformation
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information");
	FRotator AimingRotation = FRotator::ZeroRotator;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information");
	FRotator CharacterActorRotation = FRotator::ZeroRotator;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information");
	FVector Velocity = FVector::ZeroVector;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information");
	FVector RelativeVelocityDirection = FVector::ZeroVector;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information");
	FVector Acceleration = FVector::ZeroVector;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information");
	FVector MovementInput = FVector::ZeroVector;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information");
	bool bIsMoving = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information");
	bool bHasMovementInput;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information");
	float Speed = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information");
	float MovementInputAmount = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information");
	float AimYawRate = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information");
	EALS_MovementState PreviousMovementState = EALS_MovementState::None;
};

USTRUCT(BlueprintType)
struct FALSAnimConfiguration
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS|Main Configuration")
	float AnimatedWalkSpeed = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS|Main Configuration")
	float AnimatedRunSpeed = 350.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS|Main Configuration")
	float AnimatedSprintSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS|Main Configuration")
	float AnimatedCrouchSpeed = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS|Main Configuration")
	float VelocityBlendInterpSpeed = 12.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS|Main Configuration")
	float GroundedLeanInterpSpeed = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS|Main Configuration")
	float InAirLeanInterpSpeed = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS|Main Configuration")
	float SmoothedAimingRotationInterpSpeed = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS|Main Configuration")
	float InputYawOffsetInterpSpeed = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS|Main Configuration")
	float TriggerPivotSpeedLimit = 200.0f;
	
};

USTRUCT(BlueprintType)
struct FALSAnimGraphGrounded
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "ALS|Anim Graph - Grounded")
	EHipsDirection TrackedHipsDirection = EHipsDirection::F;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Grounded")
	bool bShouldMove = false; // Should be false initially

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Grounded")
	bool bRotateL = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Grounded")
	bool bRotateR = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "ALS|Anim Graph - Grounded")
	bool bPivot = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Grounded")
	float RotateRate = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Grounded")
	float RotationScale = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Grounded")
	float DiagonalScaleAmount = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Grounded")
	float WalkRunBlend = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Grounded")
	float StandingPlayRate = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Grounded")
	float CrouchingPlayRate = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Grounded")
	float StrideBlend = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Grounded")
	float FYawC = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Grounded")
	float BYawC = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Grounded")
	float LYawC = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Grounded")
	float RYawC = 0.0f;
};

USTRUCT(BlueprintType)
struct FDynamicMontageParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Dynamic Transition")
	TObjectPtr<UAnimSequenceBase> Animation = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Dynamic Transition")
	float BlendInTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Dynamic Transition")
	float BlendOutTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Dynamic Transition")
	float PlayRate = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Dynamic Transition")
	float StartTime = 0.0f;
};

USTRUCT(BlueprintType)
struct FALSAnimGraphLayerBlending
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Layer Blending")
	int32 OverlayOverrideState = 0;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Layer Blending")
	float EnableAimOffset = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Layer Blending")
	float BasePose_N = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Layer Blending")
	float BasePose_CLF = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Layer Blending")
	float Arm_L = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Layer Blending")
	float Arm_L_Add = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Layer Blending")
	float Arm_L_LS = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Layer Blending")
	float Arm_L_MS = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Layer Blending")
	float Arm_R = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Layer Blending")
	float Arm_R_Add = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Layer Blending")
	float Arm_R_LS = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Layer Blending")
	float Arm_R_MS = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Layer Blending")
	float Hand_L = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Layer Blending")
	float Hand_R = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Layer Blending")
	float Legs = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Layer Blending")
	float Legs_Add = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Layer Blending")
	float Pelvis = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Layer Blending")
	float Pelvis_Add = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Layer Blending")
	float Spine = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Layer Blending")
	float Spine_Add = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Layer Blending")
	float Head = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Anim Graph - Layer Blending")
	float Head_Add = 0.0f;	
};
