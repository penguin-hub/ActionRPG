// C135203_GraduationProject

#include "Component/GPAttributeComponent.h"
#include "Character/GPCharacter.h"
#include "Character/LevelStruct.h"
#include "Component/InventorySystemComponent.h"
#include "Engine/DataTable.h"
#include "Game/GPGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/PlayerHUD.h"

UGPAttributeComponent::UGPAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGPAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SetLevelDataTable();
}

void UGPAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGPAttributeComponent::SetAttributeByLevel()
{
	FLevelStruct* LevelStruct = LevelDataTable->FindRow<FLevelStruct>(LVNames[CurrentLV], "");
	if(LevelStruct)
	{
		SetCurrentLV(LevelStruct->LV);
		SetMaxHealth(LevelStruct->MaxHealth);
		//SetHealth(LevelStruct->MaxHealth);
		SetMaxStamina(LevelStruct->MaxStamina);
		//SetStamina(LevelStruct->MaxStamina);
		SetMaxXP(LevelStruct->MaxXP);
		if(OwnerGPCharacter)
		{
			SetPlayerHUD();
			SetInventorySize(LevelStruct);
		}
	}
}

float UGPAttributeComponent::GetHealthPercent()
{
	return Health/MaxHealth;
}

void UGPAttributeComponent::AddHealth(float HP)
{
	if(Health > 0)
		Health = FMath::Clamp(Health + HP, 0.f, MaxHealth);
}

float UGPAttributeComponent::GetStaminaPercent()
{
	return Stamina/MaxStamina;
}

void UGPAttributeComponent::UseStamina(float StaminaUse)
{
	if(Stamina > 0)
	{
		Stamina = FMath::Clamp(Stamina - StaminaUse, 0.f, MaxStamina);
	}
	else
	{
		Stamina = FMath::Clamp(Stamina - StaminaUse, 0.f, MaxStamina);
		StopRunning();
	}
}



void UGPAttributeComponent::AddXP(float XP)
{
	CurrentXP += XP;
	if(CurrentXP >= MaxXP)
	{
		CharacterLevelUP();
	}
}

void UGPAttributeComponent::AddSoul(int32 SoulNum)
{
	Soul += SoulNum;
}

void UGPAttributeComponent::ReceiveDamage(float Damage)
{
	if(Health > 0)
		Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	if(Health <= 0)
		bIsAlive = false;
}

void UGPAttributeComponent::SetLevelDataTable()
{
	if(GetWorld())
	{
		UGPGameInstance* GI = Cast<UGPGameInstance>(GetWorld()->GetGameInstance());
		if(GI)
		{
			LevelDataTable = GI->GetLevelDataTable();
		}
	}
}

void UGPAttributeComponent::StopRunning()
{
	if(OwnerGPCharacter && OwnerGPCharacter->IsRunnig == true)
	{
		OwnerGPCharacter->GetCharacterMovement()->MaxWalkSpeed /= OwnerGPCharacter->RunSpeed;
		OwnerGPCharacter->IsRunnig = false;
	}
}

void UGPAttributeComponent::CharacterLevelUP()
{
	CurrentLV++;
	CurrentXP = CurrentXP - MaxXP;
	SetAttributeByLevel();
	OwnerGPCharacter->SpawnLevelUpParticle();
	OwnerGPCharacter->PlayLevelUpSound();
}

void UGPAttributeComponent::SetPlayerHUD()
{
	if(OwnerGPCharacter->PlayerHUD)
	{
		OwnerGPCharacter->PlayerHUD->SetCurrentXPText(GetCurrentXP());
		OwnerGPCharacter->PlayerHUD->SetMaxXPText(GetMaxXP());
		OwnerGPCharacter->PlayerHUD->SetCurrentLVText(GetCurrentLV());
		OwnerGPCharacter->PlayerHUD->AddHealthBorderSize();
		OwnerGPCharacter->PlayerHUD->SetHealthBorderSize();
		OwnerGPCharacter->PlayerHUD->SetHPBarPercent(GetHealthPercent());
		OwnerGPCharacter->PlayerHUD->AddStaminaBorderSize();
		OwnerGPCharacter->PlayerHUD->SetStaminaBorderSize();
		OwnerGPCharacter->PlayerHUD->SetStaminaBarPercent(GetStaminaPercent());
	}
}

void UGPAttributeComponent::SetInventorySize(FLevelStruct* LevelStruct)
{
	if(OwnerGPCharacter->Inventory)
	{
		OwnerGPCharacter->Inventory->SetInventorySizeByLevel(LevelStruct);
	}
}