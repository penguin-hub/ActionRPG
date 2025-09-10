// C135203_GraduationProject
#pragma once

#include "CoreMinimal.h"
#include "Character/LevelStruct.h"
#include "Components/ActorComponent.h"
#include "GPAttributeComponent.generated.h"

class AGPCharacter;
class UDataTable;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRADUATIONPROJECT_API UGPAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGPAttributeComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	AGPCharacter* OwnerGPCharacter;
	UDataTable* LevelDataTable;
	//Health
	void SetAttributeByLevel();
	FORCEINLINE void SetHealth(float H){Health = H;}
	FORCEINLINE void SetMaxHealth(float MaxH){MaxHealth = MaxH;}
	float GetHealthPercent();
	void AddHealth(float HP);
	//Stamina
	FORCEINLINE float GetStamina() const {return Stamina;}
	FORCEINLINE void SetStamina(float S){Stamina = S;}
	FORCEINLINE void SetMaxStamina(float MaxS){MaxStamina = MaxS;}
	float GetStaminaPercent();
	void UseStamina(float StaminaUse);
	//XP
	FORCEINLINE float GetCurrentXP() const {return CurrentXP;}
	FORCEINLINE float GetMaxXP() const {return MaxXP;}
	FORCEINLINE void SetMaxXP(float MaxX){MaxXP = MaxX;}
	void AddXP(float XP);
	//LV
	FORCEINLINE float GetCurrentLV() const {return CurrentLV;}
	FORCEINLINE void SetCurrentLV(float LV){CurrentLV = LV;}
	//Soul
	FORCEINLINE int32 GetSoul() const {return Soul;}
	void AddSoul(int32 SoulNum);
	
	FORCEINLINE bool CheckIsAlive(){return bIsAlive;}
	void ReceiveDamage(float Damage);
private:
	UPROPERTY(EditAnywhere, Category= "Attribute")
	float Health;
	UPROPERTY(EditAnywhere, Category= "Attribute")
	float MaxHealth;
	UPROPERTY(EditAnywhere, Category= "Attribute")
	float Stamina;
	UPROPERTY(EditAnywhere, Category= "Attribute")
	float MaxStamina;
	UPROPERTY(EditAnywhere, Category= "Attribute")
	float CurrentXP;
	UPROPERTY(EditAnywhere, Category= "Attribute")
	float MaxXP;
	UPROPERTY(EditAnywhere, Category= "Attribute")
	int32 CurrentLV = 1;
	UPROPERTY(VisibleAnywhere, Category= "Attribute")
	int32 Soul;
	
	UPROPERTY(EditAnywhere, Category= "Attribute")
	TArray<FName> LVNames = {FName("0"), FName("1"), FName("2"), FName("3"), FName("4"), FName("5")};

	bool bIsAlive = true;
	void SetLevelDataTable();
	void StopRunning();
	void CharacterLevelUP();
	void SetPlayerHUD();
	void SetInventorySize(FLevelStruct* LevelStruct);
};
