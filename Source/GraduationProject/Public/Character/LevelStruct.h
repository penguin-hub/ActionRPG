// C135203_GraduationProject
#pragma once
#include "Engine/DataTable.h"
#include "LevelStruct.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FLevelStruct: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 LV;
	UPROPERTY(EditAnywhere)
	float MaxHealth;
	UPROPERTY(EditAnywhere)
	float MaxStamina;
	UPROPERTY(EditAnywhere)
	int32 InventoryMaxSlot;
	UPROPERTY(EditAnywhere)
	float MaxXP;
};

