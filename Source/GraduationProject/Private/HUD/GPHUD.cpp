// C135203_GraduationProject

#include "HUD/GPHUD.h"
#include "HUD/PlayerHUD.h"
#include "Blueprint/UserWidget.h"

void AGPHUD::BeginPlay()
{
	Super::BeginPlay();

	CreatePlayerHUDWidget();
}

void AGPHUD::CreatePlayerHUDWidget()
{
	UWorld* World = GetWorld();
	if(World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if(Controller && PlayerHUDClass)
		{
			PlayerHUD = CreateWidget<UPlayerHUD>(Controller, PlayerHUDClass);
			PlayerHUD->AddToViewport();
		}
	}
}



