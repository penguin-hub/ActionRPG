// C135203_GraduationProject
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentList.generated.h"

class UEquipmentSlot;
class UDataTable;
class AGPCharacter;
class UEquipmentListSlot;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class GRADUATIONPROJECT_API UEquipmentList : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* EquipmentListVerticalBox;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UEquipmentListSlot> EquipmentListSlotClass;
	UPROPERTY()
	UEquipmentListSlot* EquipmentListSlot;
	UPROPERTY()
	UEquipmentSlot* OwnerEquipmentSlot;
private:
	UPROPERTY()
	FName ItemName;
	UPROPERTY()
	AGPCharacter* PlayerGPCharacter;
	UDataTable* DataTable;
	UDataTable* ItemDataTable;
	void AddEquipmentListSlotToEquipmentList();
	void TransVerticalBoxPositionToMousePosition() const;
	void SetPlayerGPCharacter();
	void SetItemDataTable();
};


