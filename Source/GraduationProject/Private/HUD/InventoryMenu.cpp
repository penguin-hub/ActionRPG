// C135203_GraduationProject

#include "HUD/InventoryMenu.h"
#include "Character/GPCharacter.h"
#include "GameFramework/Character.h"
#include "HUD/InventoryGrid.h"

void UInventoryMenu::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(true);
	SetInputModeUIOnly();
	SetPlayerGPCharacter();
	SetInventoryGrid();
}

void UInventoryMenu::NativeDestruct()
{
	Super::NativeDestruct();
	
	SetInputModeGameOnly();
}

FReply UInventoryMenu::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	//i키 다시 눌렀을 떄 remove form parent
	if(InKeyEvent.GetKey() == "i")
		this->RemoveFromParent();
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UInventoryMenu::SetPlayerGPCharacter()
{
	if(GetOwningPlayer()->GetCharacter())
		PlayerGPCharacter = Cast<AGPCharacter>(GetOwningPlayer()->GetCharacter());
}

void UInventoryMenu::SetInventoryGrid()
{
	if(PlayerGPCharacter && InventoryGrid)
	{
		//Inventory의 Content 들 Grid 에 Slot 위젯 추가
		InventoryGrid->SetInventoryComponent(PlayerGPCharacter->Inventory);
	}
}

void UInventoryMenu::SetInputModeUIOnly()
{
	FInputModeUIOnly InputModeUI;
	InputModeUI.SetWidgetToFocus(this->TakeWidget());
	SetKeyboardFocus();
	if(GetOwningPlayer())
	{
		//InputMode 변경
		GetOwningPlayer()->SetInputMode(InputModeUI);
		//마우스 커서 보이게
		GetOwningPlayer()->bShowMouseCursor = true;
	}
}

void UInventoryMenu::SetInputModeGameOnly()
{
	if(GetOwningPlayer())
	{
		GetOwningPlayer()->SetInputMode(FInputModeGameOnly());
		GetOwningPlayer()->bShowMouseCursor = false;
	}
}


