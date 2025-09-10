// C135203_GraduationProject
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentMenu.generated.h"

class UDataTable;
class AGPCharacter;
class UEquipmentSlot;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class GRADUATIONPROJECT_API UEquipmentMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* VerticalBox;
	UPROPERTY(meta=(BindWidget))
	UEquipmentSlot* SwordSlot;
	UPROPERTY(meta=(BindWidget))
	UEquipmentSlot* ShieldSlot;
	UPROPERTY()
	UEquipmentSlot* SelectedSlot;
private:
	UPROPERTY()
	AGPCharacter* PlayerGPCharacter;
	UPROPERTY()
	UDataTable* ItemDataTable;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	void SetInputModeGameOnly();
	void SetItemDataTable();
	void SetInputModeUIOnly();
	void SetPlayerGPCharacter();
};
