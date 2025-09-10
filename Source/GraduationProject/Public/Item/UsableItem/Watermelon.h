// C135203_GraduationProject
#pragma once

#include "CoreMinimal.h"
#include "Interface/ItemUseInterface.h"
#include "Item/Item.h"
#include "Watermelon.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATIONPROJECT_API AWatermelon : public AItem, public IItemUseInterface
{
	GENERATED_BODY()
public:
	float UpHealth = 50.f;

	virtual void ItemUse(AActor* UsingActor) override;
};
