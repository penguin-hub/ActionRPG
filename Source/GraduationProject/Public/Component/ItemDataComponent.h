// C135203_GraduationProject
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/SlotStruct.h"
#include "ItemDataComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRADUATIONPROJECT_API UItemDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UItemDataComponent();
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	FSlotStruct ItemData;
};
