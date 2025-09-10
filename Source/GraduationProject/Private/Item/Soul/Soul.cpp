// C135203_GraduationProject

#include "Item/Soul/Soul.h"
#include "Character/GPCharacter.h"
#include "Component/GPAttributeComponent.h"
#include "HUD/PlayerHUD.h"
#include "Kismet/GameplayStatics.h"

void ASoul::ItemInteract(AActor* InteractActor)
{
	if(InteractSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), InteractSound, this->GetActorLocation());
	}
	
	AGPCharacter* GPCharacter = Cast<AGPCharacter>(InteractActor);
	if(GPCharacter)
	{
		GPCharacter->Attribute->AddSoul(SoulNum);
		GPCharacter->PlayerHUD->SetSoulText(GPCharacter->Attribute->GetSoul());
	}
	
	this->Destroy();
}
