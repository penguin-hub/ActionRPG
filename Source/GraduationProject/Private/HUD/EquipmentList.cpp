// C135203_GraduationProject

#include "HUD/EquipmentList.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Character/GPCharacter.h"
#include "Component/InventorySystemComponent.h"
#include "Components/VerticalBox.h"
#include "Game/GPGameInstance.h"
#include "HUD/EquipmentListSlot.h"
#include "HUD/EquipmentSlot.h"
#include "Item/SlotStruct.h"

void UEquipmentList::NativeConstruct()
{
	SetPlayerGPCharacter();
	SetItemDataTable();
	AddEquipmentListSlotToEquipmentList();
	TransVerticalBoxPositionToMousePosition();
	
	Super::NativeConstruct();
}

void UEquipmentList::AddEquipmentListSlotToEquipmentList()
{
	EquipmentListVerticalBox->ClearChildren();
	if(PlayerGPCharacter)
	{
		TArray<FSlotStruct> Content = PlayerGPCharacter->Inventory->Content;
		//인벤토리의 Content 중 같은 타입의 아이템들 EquipmentListSlot 추가
		for (int i = 0; i < Content.Num(); i++)
		{
			ItemName = Content[i].ItemDataTableRowHandle.RowName;
			FItemStruct* ItemStruct = ItemDataTable->FindRow<FItemStruct>(ItemName, "");
			if(ItemStruct)
			{
				if(ItemStruct->ItemType == OwnerEquipmentSlot->EquipmentType)
				{
					EquipmentListSlot = CreateWidget<UEquipmentListSlot>(GetOwningPlayer(), EquipmentListSlotClass);
					EquipmentListSlot->OwnerEquipmnetList = this;
					EquipmentListVerticalBox->AddChildToVerticalBox(EquipmentListSlot);
					EquipmentListSlot->SetEquipmentListSlot(ItemName);
				}	
			}
		}
	}
}

void UEquipmentList::TransVerticalBoxPositionToMousePosition() const
{
	float X = 0.f;
	float Y = 0.f;
	UWidgetLayoutLibrary::GetMousePositionScaledByDPI(GetOwningPlayer(), X, Y);
	const FVector2d MousePosition(X, Y);
	EquipmentListVerticalBox->SetRenderTranslation(MousePosition);
}

void UEquipmentList::SetPlayerGPCharacter()
{
	if(GetOwningPlayer()->GetCharacter())
		PlayerGPCharacter = Cast<AGPCharacter>(GetOwningPlayer()->GetCharacter());
}

void UEquipmentList::SetItemDataTable()
{
	UGPGameInstance* GPGameInstance = Cast<UGPGameInstance>(GetGameInstance());
	if(GPGameInstance)
	{
		ItemDataTable = GPGameInstance->GetItemDataTabe();
	}
}
