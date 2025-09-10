// C135203_GraduationProject
#pragma once

#include "CoreMinimal.h"
#include "Interface/ItemUseInterface.h"
#include "Item/Item.h"
#include "Clementine.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATIONPROJECT_API AClementine : public AItem, public IItemUseInterface
{
	GENERATED_BODY()

public:
	float UpHealth = 10.f;

	virtual void ItemUse(AActor* UsingActor) override;
};
