// C135203_GraduationProject
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GRADUATIONPROJECT_API IHitInterface
{
	GENERATED_BODY()

public:
	//BlueprintNativeEvent, C++에서도 블루프린트에서도 구현 가능
	UFUNCTION(BlueprintNativeEvent)
	void GetHit(const FVector& ImpactPoint);
};
