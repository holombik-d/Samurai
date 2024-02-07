#pragma once
#include "Engine/DataTable.h"
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
