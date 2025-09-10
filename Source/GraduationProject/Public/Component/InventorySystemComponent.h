// C135203_GraduationProject
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/SlotStruct.h"
#include "InventorySystemComponent.generated.h"

struct FLevelStruct;
class AItem;
class UDataTable;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRADUATIONPROJECT_API UInventorySystemComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UInventorySystemComponent();
	virtual void BeginPlay() override;

	//인벤토리의 주요 저장소, FSlotStruct 배열
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FSlotStruct> Content;
	
	void SetInventorySizeByLevel(const FLevelStruct* LevelStruct);
	//Content 내용 디버그 메시지
	void DebugShowContent();
	//인벤토리에 아이템을 추가
	UFUNCTION()
	void AddToInventory(const FSlotStruct& ItemData);
	//인벤토리에서 아이템 제거
	UFUNCTION()
	void RemoveFromInventory(int32 ContentIndex);
	//인벤토리에 슬롯 추가
	UFUNCTION()
	void AddInventorySlot(int32 AddSlotNum);
	FORCEINLINE void SetInventorySize(int32 size){InventorySize = size;}
	FORCEINLINE int32 GetInventorySize(){return InventorySize;}
protected:
	UPROPERTY(EditAnywhere)
	int32 InventorySize;
	UPROPERTY()
	UDataTable* ItemDataTable;
	//아이템 넣을 수 있는 Slot Index 찾는 함수
	UFUNCTION()
	int32 FindSlotIndex(FName ItemID);
	//해당 아이템을 하나의 슬롯에 넣을 수 있는 최대 수량
	UFUNCTION()
	int32 GetMaxStackSize(FName ItemID);
private:
	bool FindSlot = false;
	bool FindEmptySlot = false;
	
	void SetItemDataTable();
};
