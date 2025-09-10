// C135203_GraduationProject

#include "HUD/EquipmentMenu.h"
#include "Character/GPCharacter.h"
#include "Components/VerticalBox.h"
#include "Game/GPGameInstance.h"
#include "HUD/EquipmentListSlot.h"
#include "HUD/EquipmentSlot.h"

void UEquipmentMenu::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetItemDataTable();
}

void UEquipmentMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetIsFocusable(true);
	SetInputModeUIOnly();
	SetPlayerGPCharacter();
	//각 슬롯 설정
	SwordSlot->EquipmentType = EItemType::Weapon;
	SwordSlot->OwnerEquipmentMenu = this;
	ShieldSlot->EquipmentType = EItemType::Shield;
	ShieldSlot->OwnerEquipmentMenu = this;
}

void UEquipmentMenu::NativeDestruct()
{
	SetInputModeGameOnly();
	
	Super::NativeDestruct();
}

FReply UEquipmentMenu::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if(InKeyEvent.GetKey() == "e")
		RemoveFromParent();
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UEquipmentMenu::SetInputModeGameOnly()
{
	FInputModeGameOnly InputModeGame;
	if(GetOwningPlayer())
	{
		GetOwningPlayer()->SetInputMode(InputModeGame);
		GetOwningPlayer()->bShowMouseCursor = false;
	}
}

void UEquipmentMenu::SetItemDataTable()
{
	UGPGameInstance* GPGameInstance = Cast<UGPGameInstance>(GetGameInstance());
	if(GPGameInstance)
		ItemDataTable = GPGameInstance->GetItemDataTabe();
}

void UEquipmentMenu::SetInputModeUIOnly()
{
	FInputModeUIOnly InputModeUI;
	InputModeUI.SetWidgetToFocus(this->TakeWidget());
	if(GetOwningPlayer())
	{
		GetOwningPlayer()->SetInputMode(InputModeUI);
		GetOwningPlayer()->bShowMouseCursor = true;
	}
}

void UEquipmentMenu::SetPlayerGPCharacter()
{
	if(GetOwningPlayer()->GetCharacter())
		PlayerGPCharacter = Cast<AGPCharacter>(GetOwningPlayer()->GetCharacter());
}