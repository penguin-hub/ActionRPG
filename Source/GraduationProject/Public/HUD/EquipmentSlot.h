// C135203_GraduationProject
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/EItemType.h"
#include "EquipmentSlot.generated.h"

class AGPCharacter;
class UEquipmentMenu;
class UImage;
class UEquipmentList;
class UPlayerHUD;
class UButton;
/**
 * 
 */
UCLASS()
class GRADUATIONPROJECT_API UEquipmentSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	UPROPERTY(meta=(BindWidget))
	UButton* EquipmentSlotButton;
	UPROPERTY(meta=(BindWidget))
	UImage* Icon;
	UPROPERTY(EditAnywhere)
	EItemType EquipmentType;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UEquipmentList> EquipmentListClass;
	UPROPERTY()
	UEquipmentList* EquipmentList;
	UPROPERTY()
	UEquipmentMenu* OwnerEquipmentMenu;
	
	void AddEquipmentList();
	void SetIcon(UTexture2D* Image);
	//장비 중인 무기로 이미지 설정
	void SetIconAsEquippedWeapon();
private:
	UPlayerHUD* PlayerHUD;
	AGPCharacter* PlayerCharacter;
	UFUNCTION()
	void ButtonCallBack();
	void SetSelectedSlot();
	void SetPlayerHUD();
	void SetPlayerCharacter();
};


