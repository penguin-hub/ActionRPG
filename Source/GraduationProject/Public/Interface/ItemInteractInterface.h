// C135203_GraduationProject
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInteractInterface.generated.h"

UINTERFACE(MinimalAPI)
class UItemInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GRADUATIONPROJECT_API IItemInteractInterface
{
	GENERATED_BODY()
public:
	//아이템의 상호작용을 위한 인터페이스 함수
	virtual void ItemInteract(AActor* InteractActor) = 0;
};
