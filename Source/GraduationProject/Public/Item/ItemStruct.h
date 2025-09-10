// C135203_GraduationProject
#pragma once

#include "Engine/DataTable.h"
#include "ItemStruct.generated.h"

class AItem;
enum class EItemType : uint8;

USTRUCT(Atomic, BlueprintType)
struct FItemStruct: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText Name;
	UPROPERTY(EditAnywhere)
	FText Description;
	UPROPERTY(EditAnywhere)
	UTexture2D* Thumbnail;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AItem> ItemClass;
	UPROPERTY(EditAnywhere)
	int32 StackSize;
	UPROPERTY(EditAnywhere)
	float Damage;
	UPROPERTY(EditAnywhere)
	EItemType ItemType;
};

