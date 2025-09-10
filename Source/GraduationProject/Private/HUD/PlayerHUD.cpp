// C135203_GraduationProject

#include "HUD/PlayerHUD.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "HUD/EquipmentMenu.h"
#include "HUD/InventoryMenu.h"

void UPlayerHUD::AddHealthBorderSize()
{
	HealthBorderSize.X += 50.f;
}

void UPlayerHUD::SetHealthBorderSize()
{
	UCanvasPanelSlot* HealthBarSlot = Cast<UCanvasPanelSlot>(HealthBorder->Slot);
	if(HealthBarSlot)
	{
		HealthBarSlot->SetSize(HealthBorderSize);
	}
}

void UPlayerHUD::AddStaminaBorderSize()
{
	StaminaBorderSize.X += 50.f;
}

void UPlayerHUD::SetStaminaBorderSize()
{
	UCanvasPanelSlot* StaminaBarSlot = Cast<UCanvasPanelSlot>(StaminaBorder->Slot);
	if(StaminaBarSlot)
	{
		StaminaBarSlot->SetSize(StaminaBorderSize);
	}
}

void UPlayerHUD::SetHPBarPercent(float Percent)
{
	if(HPBar)
		HPBar->SetPercent(Percent);
}

void UPlayerHUD::SetStaminaBarPercent(float Percent)
{
	if(StaminaBar)
		StaminaBar->SetPercent(Percent);
}

void UPlayerHUD::SetCurrentXPText(float XP)
{
	if(CurrentXP)
		CurrentXP->SetText(FText::FromString(FString::FromInt(static_cast<int32>(XP))));
}

void UPlayerHUD::SetMaxXPText(float XP)
{
	if(MaxXP)
		MaxXP->SetText(FText::FromString(FString::FromInt(static_cast<int32>(XP))));
}

void UPlayerHUD::SetCurrentLVText(int32 LV)
{
	if(CurrentLV)
		CurrentLV->SetText(FText::FromString(FString::FromInt(LV)));
}

void UPlayerHUD::SetSoulText(int32 num)
{
	if(Soul)
		Soul->SetText(FText::FromString(FString::Printf(TEXT("%d"), num)));
}

void UPlayerHUD::DisplayInventoryMenu()
{
	if(InventoryMenuClass)
	{
		InventoryMenu = CreateWidget<UInventoryMenu>(this->GetOwningPlayer(), InventoryMenuClass);
		if(InventoryMenu)
			InventoryMenu->AddToViewport();
	}
	
}

void UPlayerHUD::DisplayEquipmentMenu()
{
	if(EquipmentMenuClass)
	{
		EquipmentMenu = CreateWidget<UEquipmentMenu>(this->GetOwningPlayer(), EquipmentMenuClass);
		if(EquipmentMenu)
			EquipmentMenu->AddToViewport();
	}
}
