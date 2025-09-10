// C135203_GraduationProject
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "InventorySlot.generated.h"

class UInventoryGrid;
class UInventoryActionMenu;
class UButton;
class UInventorySystemComponent;
class UTextBlock;
class USizeBox;
class UImage;
class UBorder;
class UDataTable;
/**
 * 
 */
UCLASS()
class GRADUATIONPROJECT_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	UBorder* BRD_Border; //선택 되었을 때 색깔 바뀐다
	UPROPERTY(meta=(BindWidget))
	UImage* IMG_Icon;
	UPROPERTY(meta=(BindWidget))
	USizeBox* BOX_Quantity;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TXT_Quantity;
	UPROPERTY(meta=(BindWidget))
	UButton* Button;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryActionMenu> InventoryActionMenuClass;
	UPROPERTY()
	UInventoryActionMenu* InventoryActionMenu;
	
	void SetInventorySlot(FName ItemID, int32 Quantity);
	
	FORCEINLINE UInventoryGrid* GetOwnerInventoryGrid(){return OwnerInventoryGrid;}
	FORCEINLINE void SetContentIndex(int32 index) {ContentIndex = index;}
	FORCEINLINE void SetOwnerInventoryGrid(UInventoryGrid* InventoryGrid) {OwnerInventoryGrid =InventoryGrid;}
private:
	UPROPERTY()
	UInventoryGrid* OwnerInventoryGrid;
	UPROPERTY()
	UInventorySystemComponent* InventorySystemComponent;
	int32 ContentIndex;
	UPROPERTY()
	UDataTable* ItemDataTable;
	UFUNCTION()
	void ButtonCallBack();
	void SetActionMenu();
	void SetItemDataTable();
};
