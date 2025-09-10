// C135203_GraduationProject

#include "HUD/EquipmentSlot.h"
#include "Character/GPCharacter.h"
#include "Component/ItemDataComponent.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Game/GPGameInstance.h"
#include "HUD/EquipmentMenu.h"
#include "HUD/EquipmentList.h"
#include "HUD/PlayerHUD.h"
#include "Item/ItemStruct.h"
#include "Item/Weapon/Weapon.h"
#include "Player/GPPlayerController.h"

void UEquipmentSlot::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetPlayerHUD();
	SetPlayerCharacter();
	//버튼 함수 바인딩
	EquipmentSlotButton->OnClicked.AddDynamic(this, &UEquipmentSlot::ButtonCallBack);
}

void UEquipmentSlot::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetIconAsEquippedWeapon();
}

void UEquipmentSlot::NativeDestruct()
{
	Super::NativeDestruct();
	if(EquipmentList)
	{
		EquipmentList->RemoveFromParent();
	}
}

void UEquipmentSlot::AddEquipmentList()
{
	EquipmentList = CreateWidget<UEquipmentList>(GetOwningPlayer(), EquipmentListClass);
	EquipmentList->OwnerEquipmentSlot = this;
	EquipmentList->AddToViewport();
}

void UEquipmentSlot::SetIcon(UTexture2D* Image)
{
	Icon->SetBrushFromTexture(Image, true);
}

void UEquipmentSlot::SetIconAsEquippedWeapon()
{
	if(EquipmentType == EItemType::Weapon)
	{
		if(PlayerCharacter && PlayerCharacter->EquippedWeapon)
		{
			FName Name = PlayerCharacter->EquippedWeapon->ItemDataComponent->ItemData.ItemDataTableRowHandle.RowName;
			UGPGameInstance* GI = Cast<UGPGameInstance>(GetGameInstance());
			SetIcon(GI->GetItemDataTabe()->FindRow<FItemStruct>(Name, "")->Thumbnail);
		}
	}
}

void UEquipmentSlot::ButtonCallBack()
{
	if(EquipmentList)
		EquipmentList->RemoveFromParent();
	SetSelectedSlot();
	AddEquipmentList();
}

void UEquipmentSlot::SetSelectedSlot()
{
	if(PlayerHUD && PlayerHUD->EquipmentMenu)
	{
		PlayerHUD->EquipmentMenu->SelectedSlot = this;
	}
}

void UEquipmentSlot::SetPlayerHUD()
{
	AGPPlayerController* Controller = Cast<AGPPlayerController>(GetOwningPlayer());
	if(Controller)
	{
		PlayerHUD = Controller->HavingGPCharacter->PlayerHUD;
	}
}

void UEquipmentSlot::SetPlayerCharacter()
{
	if(PlayerHUD && PlayerHUD->GetOwningPlayerPawn())
	{
		PlayerCharacter = Cast<AGPCharacter>(PlayerHUD->GetOwningPlayerPawn());
	}
}