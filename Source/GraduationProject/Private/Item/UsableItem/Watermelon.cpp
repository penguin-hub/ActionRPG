// C135203_GraduationProject

#include "Item/UsableItem/Watermelon.h"
#include "Character/GPCharacter.h"
#include "Component/GPAttributeComponent.h"
#include "HUD/PlayerHUD.h"

void AWatermelon::ItemUse(AActor* UsingActor)
{
	AGPCharacter* PlayerCharacter = Cast<AGPCharacter>(UsingActor);
	if(PlayerCharacter && PlayerCharacter->Attribute)
	{
		PlayerCharacter->Attribute->AddHealth(UpHealth);
		if(PlayerCharacter->PlayerHUD)
		{
			PlayerCharacter->PlayerHUD->SetHPBarPercent(PlayerCharacter->Attribute->GetHealthPercent());
		}
	}
}
