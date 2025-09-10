// C135203_GraduationProject

#include "HUD/EquipmentListSlot.h"
#include "Character/GPCharacter.h"
#include "Player/GPPlayerController.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Game/GPGameInstance.h"
#include "HUD/EquipmentList.h"
#include "HUD/EquipmentSlot.h"
#include "Item/ItemStruct.h"
#include "Item/Weapon/Weapon.h"

void UEquipmentListSlot::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	SetItemDataTable();
	SetPlayerHUD();
	SetPlayerCharacter();
	//버튼에 함수 바인딩
	Button->OnClicked.AddDynamic(this, &UEquipmentListSlot::ButtonCallBack);
}

void UEquipmentListSlot::ButtonCallBack()
{
	if(UWorld* World = GetWorld())
	{
		//WeaponClass Spawn
		AWeapon* Weapon = World->SpawnActor<AWeapon>(EquipmentListSlotItemStruct->ItemClass);
		if(Weapon)
		{
			//Arm 중일 때 오른손에 Equip
			if(PlayerCharacter->GetCharacterState() >= ECharacterState::EquippedWeapon)
				Weapon->Equip(GetOwningPlayer()->GetCharacter()->GetMesh(), FName("RightHandSocket"), GetOwningPlayer()->GetCharacter(),GetOwningPlayer()->GetCharacter());
			//DisArm 중일 때 등에 Equip
			else if(PlayerCharacter->GetCharacterState() == ECharacterState::Unequipped)
				Weapon->Equip(GetOwningPlayer()->GetCharacter()->GetMesh(), FName("SpineSocket"), GetOwningPlayer()->GetCharacter(),GetOwningPlayer()->GetCharacter());
			if(PlayerCharacter->EquippedWeapon)
				PlayerCharacter->EquippedWeapon->Destroy();
			//EquippedWeapon으로 설정
			PlayerCharacter->EquippedWeapon = Weapon;
		}
	}
	//OwnerEquipmentSlot의 이미지 변경
	OwnerEquipmnetList->OwnerEquipmentSlot->SetIconAsEquippedWeapon();
	OwnerEquipmnetList->RemoveFromParent();
}

void UEquipmentListSlot::SetEquipmentListSlot(FName ItemID)
{
	if(ItemDataTable)
	{
		EquipmentListSlotItemStruct = ItemDataTable->FindRow<FItemStruct>(ItemID, "");
		if(EquipmentListSlotItemStruct)
		{
			//이미지와 텍스트 해당 아이템 데이터로 변경
			Thumbnail->SetBrushFromTexture(EquipmentListSlotItemStruct->Thumbnail, true);
			SlotText->SetText(FText::FromName(ItemID));
		}
	}
}

void UEquipmentListSlot::SetPlayerHUD()
{
	AGPPlayerController* Controller = Cast<AGPPlayerController>(GetOwningPlayer());
	if(Controller)
	{
		PlayerHUD = Controller->HavingGPCharacter->PlayerHUD;
	}
}

void UEquipmentListSlot::SetPlayerCharacter()
{
	AGPPlayerController* Controller = Cast<AGPPlayerController>(GetOwningPlayer());
	if(Controller)
	{
		PlayerCharacter = Controller->HavingGPCharacter;
	}
}

void UEquipmentListSlot::SetItemDataTable()
{
	UGPGameInstance* GPGameInstance = Cast<UGPGameInstance>(GetGameInstance());
	if(GPGameInstance)
	{
		ItemDataTable = GPGameInstance->GetItemDataTabe();
	}
}