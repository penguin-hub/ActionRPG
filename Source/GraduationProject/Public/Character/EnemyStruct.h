// C135203_GraduationProject
#pragma once
#include "Engine/DataTable.h"
#include "EnemyStruct.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FEnemyStruct: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float MaxHealth;
	UPROPERTY(EditAnywhere)
	float GiveXP;
};

