// C135203_GraduationProject

#include "HUD/InventoryActionMenu.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Character/CharacterType.h"
#include "Character/GPCharacter.h"
#include "Component/InventorySystemComponent.h"
#include "Component/ItemDataComponent.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Game/GPGameInstance.h"
#include "HUD/InventoryGrid.h"
#include "HUD/InventorySlot.h"
#include "Interface/ItemUseInterface.h"
#include "Item/Item.h"
#include "Item/Weapon/Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/GPPlayerController.h"

void UInventoryActionMenu::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	SetItemDataTable();
	SetPlayerCharacter();
	//버튼 함수 바인딩
	UseButton->OnClicked.AddDynamic(this, &UInventoryActionMenu::UseButtonCallBack);
	DropButton->OnClicked.AddDynamic(this, &UInventoryActionMenu::DropButtonCallBack);
}

void UInventoryActionMenu::NativeConstruct()
{
	Super::NativeConstruct();
	SetItemStruct();
	SetVerticalBoxOnMousePosition();
}

void UInventoryActionMenu::SetDescription()
{
	if(ItemStruct)
		Description->SetText(ItemStruct->Description);
}

void UInventoryActionMenu::UseButtonCallBack()
{
	if(ItemStruct)
	{
		//WeaponClass Spawn
		AItem* Item = GetWorld()->SpawnActor<AItem>(ItemStruct->ItemClass);
		if(Item)
		{
			//보이지 않게 Set
			Item->SetHidden(true);
			Item->SetActorLocation(PlayerCharacter->GetActorLocation());
			//UsableItem 일 경우 Use
			if(IItemUseInterface* UsableItem  = Cast<IItemUseInterface>(Item))
			{
				UsableItem->ItemUse(PlayerCharacter);
				//UseSound 재생
				if(Item->UseSound)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), Item->UseSound, Item->GetActorLocation());
				}
				//인벤토리에서 제거 및 인벤토리슬롯 UI 갱신
				AbandonFromInventory();
				Item->Destroy();
				this->RemoveFromParent();
			}
		}
	}
}

void UInventoryActionMenu::DropButtonCallBack()
{
	//Drop 가능한 공간 있는지 확인
	FHitResult SphereHitResult;
	DoSphereTraceToCheckCanDrop(SphereHitResult);
	if(SphereHitResult.GetActor())
	{
		//Can't Drop 텍스트 위젯 화면에 나타나게
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Can't Drop"));
		return;
	}
	//아이템 월드에 Spawn
	if(ItemStruct)
	{
		SpawnItemClass();
		if(IsEquippedWeapon())
		{
			RemoveEquippedWeapon();
		}
	}
	//인벤토리에서 제거 및 인벤토리슬롯 UI 갱신
	AbandonFromInventory();
	
	this->RemoveFromParent();
}

void UInventoryActionMenu::SetItemDataTable()
{
	UGPGameInstance* GameInstance = Cast<UGPGameInstance>(GetGameInstance());
	if(GameInstance)
	{
		ItemDataTable = GameInstance->GetItemDataTabe();
	}
}

void UInventoryActionMenu::SetItemStruct()
{
	ItemStruct = ItemDataTable->FindRow<FItemStruct>(InventorySystemComponent->Content[ContentIndex].ItemDataTableRowHandle.RowName, "");
}

void UInventoryActionMenu::SetPlayerCharacter()
{
	AGPPlayerController* Controller = Cast<AGPPlayerController>(GetOwningPlayer());
	if(Controller)
	{
		PlayerCharacter = Controller->HavingGPCharacter;
	}
}

void UInventoryActionMenu::SetVerticalBoxOnMousePosition()
{
	float X = 0.f;
	float Y = 0.f;
	UWidgetLayoutLibrary::GetMousePositionScaledByDPI(GetOwningPlayer(), X, Y);
	const FVector2d MousePosition(X,Y);
	VerticalBox->SetRenderTranslation(MousePosition);
}

void UInventoryActionMenu::DoSphereTraceToCheckCanDrop(FHitResult& SphereHitResult)
{
	const FVector Start = PlayerCharacter->GetActorLocation();
	const FVector Forward = PlayerCharacter->GetActorForwardVector();
	const FVector End = Start + Forward*100.f;

	//TArray 동적으로 크기 변하는 배열, unreal engine built-in array type
	//Trace 시에 전달 할 인자 중 Ignore 할 Actor들을 전달하기 위해 생성
	TArray<AActor*> ActorsToIgnore;
	//자기 자신과 Hit 하지 않도록 추가
	ActorsToIgnore.Add(PlayerCharacter);
	
	//Trace 수행
	UKismetSystemLibrary::SphereTraceSingle(
		this,
		Start,
		End,
		10.f,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		SphereHitResult,
		true
	);
}

void UInventoryActionMenu::SpawnItemClass()
{
	AItem* Item = GetWorld()->SpawnActor<AItem>(ItemStruct->ItemClass);
	if(Item)
	{
		Item->SetActorLocation(PlayerCharacter->GetActorLocation() + PlayerCharacter->GetActorForwardVector()*100.f);
		if(Item->DropSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), Item->DropSound, Item->GetActorLocation());
		}
	}
}

bool UInventoryActionMenu::IsEquippedWeapon()
{
	return PlayerCharacter->EquippedWeapon &&
		PlayerCharacter->EquippedWeapon->ItemDataComponent->ItemData.ItemDataTableRowHandle.RowName ==
		InventorySystemComponent->Content[ContentIndex].ItemDataTableRowHandle.RowName;
}

void UInventoryActionMenu::RemoveEquippedWeapon()
{
	PlayerCharacter->EquippedWeapon->Destroy();
	PlayerCharacter->EquippedWeapon = nullptr;
	PlayerCharacter->SetCharacterState(ECharacterState::Unequipped);
}

void UInventoryActionMenu::AbandonFromInventory()
{
	if(InventorySystemComponent && OwnerInventorySlot)
	{
		InventorySystemComponent->RemoveFromInventory(ContentIndex);
		OwnerInventorySlot->SetInventorySlot(InventorySystemComponent->Content[ContentIndex].ItemDataTableRowHandle.RowName, InventorySystemComponent->Content[ContentIndex].Quantity);
	}
}