// C135203_GraduationProject
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

class UBorder;
class UEquipmentMenu;
class UInventoryMenu;
class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class GRADUATIONPROJECT_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UEquipmentMenu> EquipmentMenuClass;
	UPROPERTY()
	UEquipmentMenu* EquipmentMenu;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventoryMenu> InventoryMenuClass;
	UPROPERTY()
	UInventoryMenu* InventoryMenu;
	
	UPROPERTY(meta=(BindWidget))
	UProgressBar* HPBar;
	UPROPERTY(meta=(BindWidget))
	UProgressBar* StaminaBar;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CurrentXP;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* MaxXP;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CurrentLV;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* Soul;
	UPROPERTY(meta=(BindWidget))
	UBorder* HealthBorder;
	UPROPERTY(meta=(BindWidget))
	UBorder* StaminaBorder;
	
	FVector2d HealthBorderSize = FVector2d(500.f, 50.f);
	FVector2d StaminaBorderSize = FVector2d(500.f, 50.f);

	void AddHealthBorderSize();
	void SetHealthBorderSize();
	void AddStaminaBorderSize();
	void SetStaminaBorderSize();
	
	void SetHPBarPercent(float Percent);
	void SetStaminaBarPercent(float Percent);
	
	void SetCurrentXPText(float XP);
	void SetMaxXPText(float XP);
	void SetCurrentLVText(int32 LV);
	void SetSoulText(int32 num);
	
	//메뉴 추가
	UFUNCTION()
	void DisplayInventoryMenu();
	UFUNCTION()
	void DisplayEquipmentMenu();
};
