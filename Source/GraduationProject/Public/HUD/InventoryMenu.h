// C135203_GraduationProject
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryMenu.generated.h"

class AGPCharacter;
class UInventorySlot;
class UInventoryGrid;
/**
 * 
 */
UCLASS()
class GRADUATIONPROJECT_API UInventoryMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(meta=(BindWidget))
	UInventoryGrid* InventoryGrid;
private:
	UPROPERTY()
	int32 SelectedItemIndex = 0;
	UPROPERTY()
	UInventorySlot* SelectedSlot;
	UPROPERTY()
	AGPCharacter* PlayerGPCharacter;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	void SetPlayerGPCharacter();
	void SetInventoryGrid();
	void SetInputModeUIOnly();
	void SetInputModeGameOnly();
};
