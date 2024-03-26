#pragma once

#include "CoreMinimal.h"
#include "Character/CustomCharacterMovementComponent.h"
#include "Enums/MovementEnums.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class SAMURAI_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Constructors and initialization functions
	AMainCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void PostInitializeComponents() override;

	// Essential movement-related functions
	UFUNCTION(BlueprintCallable, Category = "Movement")
	
	FORCEINLINE class UCustomCharacterMovementComponent* GetCustomMovementComponent() const
	{
		return CustomCharacterMovementComponent;
	}
	
	UFUNCTION(BlueprintCallable, Category = "MovementSystem")
	EALS_Gait GetAllowedGait() const;
	
	UFUNCTION(BlueprintCallable, Category = "MovementSystem")
	EALS_Gait GetActualGait(EALS_Gait AllowedGait) const;
	
	UFUNCTION(BlueprintCallable, Category = "MovementSystem")
	void SetGait(EALS_Gait NewGait);

protected:
	// Initialization and event functions
	virtual void BeginPlay() override;
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
	virtual void OnStanceChanged();

	// Internal update and control functions
	void SetEssentialValues(float DeltaTime);
	void CacheValues();
	void ForceUpdateCharacterState();
	void SetMovementModel();
	void SetRotationMode(const ERotationMode NewRotationMode, bool bForce);
	void SetDesiredRotationMode(ERotationMode NewRotationMode);
	void UpdateCharacterMovement();

	// Essential information properties
	UPROPERTY(BlueprintReadOnly, Category = "Essential Information")
	FVector CurrentAcceleration = FVector::ZeroVector;
	FVector Acceleration = FVector::ZeroVector;
	FVector PreviousVelocity = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadOnly, Category = "Essential Information")
	FRotator ControlRotation = FRotator::ZeroRotator;
	FRotator LastMovementInputRotation = FRotator::ZeroRotator;
	FRotator AimingRotation = FRotator::ZeroRotator;
	FRotator LastVelocityRotation = FRotator::ZeroRotator;
	FRotator TargetRotation = FRotator::ZeroRotator;

	UPROPERTY(BlueprintReadOnly, Category = "Essential Information")
	float EasedMaxAcceleration = 0.0f;
	float Speed = 0.0f;
	float PreviousAimYaw = 0.0f;
	float MovementInputAmount = 0.0f;

	// Input-related properties
	float LastStanceInputTime = 0.0f;
	float LookLeftRightRate = 1.25f;
	float LookUpDownRate = 1.25f;
	float AimYawRate = 0.0f;
	bool bIsMoving = false;
	bool CanSprint() const;
	UPROPERTY(EditDefaultsOnly, Category = "ALS|Input", BlueprintReadOnly)
	bool bHasMovementInput = false;

	// State-related properties
	UPROPERTY(BlueprintReadOnly, Category = "ALS|State Values")
	EALS_Gait Gait = EALS_Gait::Walking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Input")
	EALS_Gait DesiredGait = EALS_Gait::Running;

	// Stance-related properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Input")
	EALS_Stance Stance = EALS_Stance::Standing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Input")
	EALS_Stance DesiredStance = EALS_Stance::Standing;

	// Rotation mode properties
	UPROPERTY(BlueprintReadOnly, Category = "ALS|State Values")
	ERotationMode DesiredRotationMode = ERotationMode::LookingDirection;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|State Values")
	ERotationMode RotationMode = ERotationMode::LookingDirection;

	// Movement state properties
	UPROPERTY(BlueprintReadOnly, Category = "ALS|State Values")
	EALS_MovementState MovementState = EALS_MovementState::None;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|State Values")
	EALS_MovementState PrevMovementState = EALS_MovementState::None;

	// Movement system settings
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|Movement System")
	FDataTableRowHandle MovementModel;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Movement System")
	FMovementStateSettings MovementData;

	// Essential movement system functions
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	FMovementSettings GetTargetMovementSettings() const;

	UPROPERTY()
	TObjectPtr<UCustomCharacterMovementComponent> CustomCharacterMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS|State Values")
	EALSOverlayState OverlayState = EALSOverlayState::Default;

	void SetOverlayState(const EALSOverlayState NewState, bool bForce);

public:
	// Tick function and input handling
	virtual void Tick(float DeltaTime) override;

	// Input handling functions
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input")
	void ForwardMovementAction(float Value);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Input")
	void RightMovementAction(float Value);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Input")
	void CameraUpAction(float Value);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Input")
	void CameraRightAction(float Value);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Input")
	void JumpAction(bool bValue);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Input")
	void SprintAction(bool bValue);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Input")
	void StanceAction();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Input")
	void WalkAction();

	// Character state setting functions
	UFUNCTION(BlueprintCallable, Category = "Character States")
	void SetDesiredGait(EALS_Gait NewGait);
	
	UFUNCTION(BlueprintCallable, Category = "Character States")
	void SetDesireStance(EALS_Stance NewStance);
	
	UFUNCTION(BlueprintCallable, Category = "Character States")
	void SetStance(EALS_Stance NewStance, bool bForce = false);
	
	UFUNCTION(BlueprintCallable, Category = "Character States")
	
	void SetMovementState(EALS_MovementState NewMovementState);
	UFUNCTION(BlueprintCallable, Category = "Character States")
	
	EALS_Stance GetStance();

	//Character essential getters
	float GetMovementInputAmount();
	float GetSpeed();
	float GetAimYawRate();
	bool GatHasMovementInput();
	bool GetIsMoving();
	FVector GetAcceleration();
	FVector GetMovementInput();
	FRotator GetRotation();
	FRotator GetAimRotation();
	EALS_MovementState GetPrevMovementState();
	EALS_MovementState GetMovementState();
	EALS_Gait GetGait();
	EALSOverlayState GetOverlayState() const;
};
