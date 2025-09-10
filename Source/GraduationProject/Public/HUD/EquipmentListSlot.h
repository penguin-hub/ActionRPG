// C135203_GraduationProject
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/ItemStruct.h"
#include "EquipmentListSlot.generated.h"

class UEquipmentList;
class AGPCharacter;
struct FItemStruct;
class UDataTable;
class UButton;
class UTextBlock;
class UImage;
class UPlayerHUD;
/**
 * 
 */
UCLASS()
class GRADUATIONPROJECT_API UEquipmentListSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;

	UPROPERTY()
	UEquipmentList* OwnerEquipmnetList;
	UPROPERTY(meta=(BindWidget))
	UImage* Thumbnail;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* SlotText;
	UPROPERTY(meta=(BindWidget))
	UButton* Button;
	
	UFUNCTION()
	void ButtonCallBack();
	void SetEquipmentListSlot(FName ItemID);
private:
	UPROPERTY()
	UDataTable* ItemDataTable;
	AGPCharacter* PlayerCharacter;
	UPlayerHUD* PlayerHUD;
	FItemStruct* EquipmentListSlotItemStruct;
	
	void SetPlayerHUD();
	void SetPlayerCharacter();
	void SetItemDataTable();
};
