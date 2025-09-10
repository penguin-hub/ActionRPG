// C135203_GraduationProject
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/HitInterface.h"
#include "BreakableActor.generated.h"

class AItem;
class UCapsuleComponent;
class UGeometryCollectionComponent;

UCLASS()
class GRADUATIONPROJECT_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();
	
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* Capsule;
	UPROPERTY(EditAnywhere)
	USoundBase* BreakSound;
private:
	bool bBroken = false;
	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* GeometryCollection;
	//해당 BreakableActor 에서 Spawn 될 수 있는 아이템 UClass들의 배열
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AItem>> ItemClassArray;
	//ItemClassArray 중 랜덤으로 하나의 아이템을 Spawn
	void SpawnItemClass();
	//BreakSound를 재생
	void PlayBreakSound(const FVector& ImpactPoint);
};
