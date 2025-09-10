// C135203_GraduationProject

#include "HUD/InventoryGrid.h"
#include "Component/InventorySystemComponent.h"
#include "Components/WrapBox.h"
#include "HUD/InventorySlot.h"

void UInventoryGrid::SetInventoryComponent(UInventorySystemComponent* Inventory)
{
	AddInventorySlotToWrapBox(Inventory);
}

void UInventoryGrid::AddInventorySlotToWrapBox(UInventorySystemComponent* Inventory)
{
	if(Inventory)
	{
		//리셋
		BOX_Grid->ClearChildren();
		//인벤토리 Content 의 Slot 들 추가
		for (int i = 0; i < Inventory->Content.Num() ; i++)
		{
			if(InventoySlotClass)
			{
				InventorySlot = CreateWidget<UInventorySlot>(this, InventoySlotClass);
				if(InventorySlot)
				{
					InventorySlot->SetInventorySlot(Inventory->Content[i].ItemDataTableRowHandle.RowName,
						Inventory->Content[i].Quantity);
					InventorySlot->SetContentIndex(i);
					InventorySlot->SetOwnerInventoryGrid(this);
					BOX_Grid->AddChildToWrapBox(InventorySlot);
				}
			}
		}
	}
}
