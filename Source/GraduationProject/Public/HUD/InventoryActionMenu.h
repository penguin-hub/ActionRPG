// C135203_GraduationProject
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/ItemStruct.h"
#include "InventoryActionMenu.generated.h"

class UTextBlock;
class AGPCharacter;
struct FItemStruct;
class UInventorySlot;
class UInventorySystemComponent;
class UButton;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class GRADUATIONPROJECT_API UInventoryActionMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* VerticalBox;
	UPROPERTY(meta=(BindWidget))
	UButton* UseButton;
	UPROPERTY(meta=(BindWidget))
	UButton* DropButton;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* Description;

	FORCEINLINE void SetInventorySystemComponent(UInventorySystemComponent* ISC){InventorySystemComponent = ISC;}
	FORCEINLINE void SetContentIndex(int32 index){ContentIndex = index;}
	FORCEINLINE void SetOwnerInventorySlot(UInventorySlot* InventorySlot) {OwnerInventorySlot = InventorySlot;}
	void SetDescription();
private:
	FItemStruct* ItemStruct;
	UPROPERTY()
	UInventorySlot* OwnerInventorySlot;
	UPROPERTY()
	UInventorySystemComponent* InventorySystemComponent;
	int32 ContentIndex;
	UPROPERTY()
	UDataTable* ItemDataTable;
	UPROPERTY()
	AGPCharacter* PlayerCharacter;

	UFUNCTION()
	void UseButtonCallBack();
	UFUNCTION()
	void DropButtonCallBack();
	void SetItemDataTable();
	void SetItemStruct();
	void SetPlayerCharacter();
	void SetVerticalBoxOnMousePosition();
	void DoSphereTraceToCheckCanDrop(FHitResult& SphereHitResult);
	void SpawnItemClass();
	bool IsEquippedWeapon();
	void RemoveEquippedWeapon();
	void AbandonFromInventory();
};
