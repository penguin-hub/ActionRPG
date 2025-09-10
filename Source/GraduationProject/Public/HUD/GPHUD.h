// C135203_GraduationProject
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GPHUD.generated.h"

class UPlayerHUD;
/**
 * 
 */
UCLASS()
class GRADUATIONPROJECT_API AGPHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	FORCEINLINE UPlayerHUD* GetPlayerHUD() const {return PlayerHUD;}
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerHUD> PlayerHUDClass;
	UPROPERTY()
	UPlayerHUD* PlayerHUD;

	void CreatePlayerHUDWidget();
};
