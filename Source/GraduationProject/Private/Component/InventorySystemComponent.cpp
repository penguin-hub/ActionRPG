// C135203_GraduationProject
#include "Component/InventorySystemComponent.h"

#include "Character/LevelStruct.h"
#include "Component/ItemDataComponent.h"
#include "Game/GPGameInstance.h"
#include "Item/Item.h"
#include "Engine/World.h"
#include "Item/ItemStruct.h"

UInventorySystemComponent::UInventorySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	SetInventorySize(InventorySize);
	Content.Init(FSlotStruct(), InventorySize);
}

void UInventorySystemComponent::BeginPlay()
{
	SetItemDataTable();
	
	Super::BeginPlay();
}

void UInventorySystemComponent::SetInventorySizeByLevel(const FLevelStruct* LevelStruct)
{
	int32 CurrentInventorySize = GetInventorySize();
	SetInventorySize(LevelStruct->InventoryMaxSlot);
	AddInventorySlot(LevelStruct->InventoryMaxSlot - CurrentInventorySize);
}

void UInventorySystemComponent::DebugShowContent()
{
	for (int i = 0; i < Content.Num() ; i++)
	{
		FName Name = Content[i].ItemDataTableRowHandle.RowName;
		FItemStruct* ItemStruct = ItemDataTable->FindRow<FItemStruct>(Name, "");
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, Name.ToString());
			if(ItemStruct)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "GetRow");
			}
		}
	}
}

void UInventorySystemComponent::AddToInventory(const FSlotStruct& ItemData)
{
	//ItemData.Quantity 만큼 아이템 추가
	for (int i = 0; i < ItemData.Quantity ; i++)
	{
		int32 AvailableSlotIndex = FindSlotIndex(ItemData.ItemDataTableRowHandle.RowName);
		//Slot 찾은 경우
		if(AvailableSlotIndex >= 0)
		{
			//새로운 비어있는 Slot 에 추가
			if(FindEmptySlot)
			{
				Content[AvailableSlotIndex].ItemDataTableRowHandle.RowName = ItemData.ItemDataTableRowHandle.RowName;
				Content[AvailableSlotIndex].Quantity += 1;
				//if(GEngine)
					//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("ADD Empty SLOT %d"), AvailableSlotIndex));
			}
			//이미 있는 Slot 에 추가
			else if(FindSlot)
			{
				Content[AvailableSlotIndex].Quantity += 1;
				//if(GEngine)
					//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("ADD Present SLOT"));
			}
		}
		//슬롯 못 찾음
		else
		{
			if(GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("NO SLOT"));
		}
	}
}

void UInventorySystemComponent::RemoveFromInventory(int32 ContentIndex)
{
	if(Content[ContentIndex].Quantity > 1)
		Content[ContentIndex].Quantity--;
	//해당 슬롯의 아이템이 마지막 하나 일때
	else if(Content[ContentIndex].Quantity == 1)
	{
		Content[ContentIndex].Quantity = 0;
		FDataTableRowHandle EmptyDataTableRowHandle;
		Content[ContentIndex].ItemDataTableRowHandle = EmptyDataTableRowHandle;
	}
}

void UInventorySystemComponent::AddInventorySlot(int32 AddSlotNum)
{
	for(int i = 0; i < AddSlotNum; i++)	
		Content.AddDefaulted();
}

int32 UInventorySystemComponent::FindSlotIndex(FName ItemID)
{
	//Content배열에서 아이템 슬롯 찾기
	//배열 for loop 돌면서 ItemID와 슬롯의 ItemID 같은지 비교
	//아이템의 스택 사이즈를 통해 한 슬롯에 몇 개나 쌓을 수 있는지 MaxSize 구함
	
	//아이템 StackSize 보다 Quantity 작은 같은 종류 아이템 슬롯 찾는 경우
	for (int i = 0; i < InventorySize ; i++)
	{
		
		if(Content[i].ItemDataTableRowHandle.RowName == ItemID)
		{
			if(Content[i].Quantity < GetMaxStackSize(ItemID))
			{
				//이미 있는 종류 슬롯 찾음
				FindSlot = true;
				FindEmptySlot = false;
				return i;
			}
		}
	}
	//위의 경우가 없어 비어 있는 슬롯 찾는 경우
	for (int i = 0; i < InventorySize ; i++)
	{
		if(Content[i].Quantity == 0)
		{
			FindEmptySlot = true;
			FindSlot = false;
			return i;
		}
	}
	//그 어떤 가능한 슬롯도 찾지 못한 경우
	FindSlot = false;
	FindEmptySlot = false;
	return -1;
}

int32 UInventorySystemComponent::GetMaxStackSize(FName ItemID)
{
	FItemStruct* ItemStruct = ItemDataTable->FindRow<FItemStruct>(ItemID, "");
	if(ItemStruct)
		return ItemStruct->StackSize;
	else
		return -1;
}

void UInventorySystemComponent::SetItemDataTable()
{
	UGPGameInstance* GI = Cast<UGPGameInstance>(GetWorld()->GetGameInstance());
	if(GI)
	{
		ItemDataTable = GI->GetItemDataTabe();
	}
}