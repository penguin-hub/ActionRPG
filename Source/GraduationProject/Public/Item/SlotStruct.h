// C135203_GraduationProject
#pragma once

#include "Engine/DataTable.h"
#include "SlotStruct.generated.h"

USTRUCT(Blueprintable)
struct FSlotStruct
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FDataTableRowHandle ItemDataTableRowHandle;
	UPROPERTY(EditAnywhere)
	int32 Quantity = 0;
};
