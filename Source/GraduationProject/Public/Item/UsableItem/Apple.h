// C135203_GraduationProject
#pragma once

#include "CoreMinimal.h"
#include "Interface/ItemUseInterface.h"
#include "Item/Item.h"
#include "Apple.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATIONPROJECT_API AApple : public AItem, public IItemUseInterface
{
	GENERATED_BODY()
public:
	float UpHealth = 20.f;
	
	virtual void ItemUse(AActor* UsingActor) override;
};
