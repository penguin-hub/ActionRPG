// C135203_GraduationProject
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryGrid.generated.h"

class UInventorySystemComponent;
class UInventorySlot;
class UWrapBox;
/**
 * 
 */
UCLASS()
class GRADUATIONPROJECT_API UInventoryGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	UWrapBox* BOX_Grid;
	
	UFUNCTION()
	void SetInventoryComponent(UInventorySystemComponent* Inventory);
	void AddInventorySlotToWrapBox(UInventorySystemComponent* Inventory);
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventorySlot> InventoySlotClass;
	UPROPERTY()
	UInventorySlot* InventorySlot;
};
