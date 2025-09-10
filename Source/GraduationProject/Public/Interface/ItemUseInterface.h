// C135203_GraduationProject

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemUseInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemUseInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GRADUATIONPROJECT_API IItemUseInterface
{
	GENERATED_BODY()

public:
	//아이템의 Use를 위한 인터페이스 함수
	virtual void ItemUse(AActor* UsingActor) = 0;
};
