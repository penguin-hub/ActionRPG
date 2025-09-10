// C135203_GraduationProject
#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "Soul.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATIONPROJECT_API ASoul : public AItem
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	int32 SoulNum;
	
	virtual void ItemInteract(AActor* InteractActor) override;
};
