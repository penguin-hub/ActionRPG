// C135203_GraduationProject

#include "HUD/InventorySlot.h"
#include "Character/GPCharacter.h"
#include "Component/InventorySystemComponent.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Engine/DataTable.h"
#include "Game/GPGameInstance.h"
#include "HUD/InventoryActionMenu.h"
#include "Item/ItemStruct.h"

void UInventorySlot::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	SetItemDataTable();
	//버튼 함수 바인딩
	Button->OnClicked.AddDynamic(this, &UInventorySlot::ButtonCallBack);
}

void UInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();
	
	AGPCharacter* GPCharacter = Cast<AGPCharacter>(GetOwningPlayer()->GetCharacter());
	if(GPCharacter)
	{
		InventorySystemComponent = GPCharacter->Inventory;
	}
}

void UInventorySlot::SetInventorySlot(FName ItemID, int32 Quantity)
{
	if(ItemDataTable)
	{
		FItemStruct* ItemStruct = ItemDataTable->FindRow<FItemStruct>(ItemID, "");
		if(ItemStruct)
		{
			//아이템 있을 경우 이미지와 수량 텍스트 설정
			IMG_Icon->SetBrushFromTexture(ItemStruct->Thumbnail, true);
			TXT_Quantity->SetText(FText::FromString(FString::FromInt(Quantity)));
			BOX_Quantity->SetVisibility(ESlateVisibility::Visible);
			IMG_Icon->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			BOX_Quantity->SetVisibility(ESlateVisibility::Hidden);
			IMG_Icon->SetVisibility(ESlateVisibility::Hidden);
		}
	}	
}

void UInventorySlot::ButtonCallBack()
{
	if(InventoryActionMenu)
		InventoryActionMenu->RemoveFromParent();
	if(InventorySystemComponent->Content[ContentIndex].Quantity > 0)
	{
		//InventoryActionMenu 생성
		InventoryActionMenu = CreateWidget<UInventoryActionMenu>(GetOwningPlayer(), InventoryActionMenuClass);
		SetActionMenu();
		InventoryActionMenu->AddToViewport();
		InventoryActionMenu->SetDescription();
	}
}

void UInventorySlot::SetActionMenu()
{
	InventoryActionMenu->SetInventorySystemComponent(InventorySystemComponent);
	InventoryActionMenu->SetContentIndex(ContentIndex);
	InventoryActionMenu->SetOwnerInventorySlot(this);
}

void UInventorySlot::SetItemDataTable()
{
	UGPGameInstance* GPGameInstance = Cast<UGPGameInstance>(GetGameInstance());
	if(GPGameInstance)
		ItemDataTable = GPGameInstance->GetItemDataTabe();
}

