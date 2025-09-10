// C135203_GraduationProject
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ItemInteractInterface.h"
#include "Item.generated.h"

class UShowTextComponent;
class UNiagaraComponent;
class UItemDataComponent;
class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class GRADUATIONPROJECT_API AItem : public AActor, public IItemInteractInterface
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void BeginPlay() override;
	
	//아이템 데이터 저장 컴포넌트
	UPROPERTY(VisibleAnywhere)
	UItemDataComponent* ItemDataComponent;
	UPROPERTY(EditAnywhere)
	USoundBase* UseSound;
	UPROPERTY(EditAnywhere)
	USoundBase* DropSound;
	
	//ItemInteractInterface 함수
	virtual void ItemInteract(AActor* InteractActor) override;
protected:
	//오버랩 시 표시되는 텍스트
	UPROPERTY(VisibleAnywhere)
	UShowTextComponent* ShowTextComponent;
	//아이템의 스태틱 메쉬 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;
	//구(Sphere) 콜리전 컴포넌트
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* ItemEffect;
	UPROPERTY(EditAnywhere)
	USoundBase* InteractSound;
	
	//PrimitiveComponent.h 에 선언되어 있는 delegate, FComponentBeginOverlapSignature타입의 OnComponentBeginOverlap를 참조하여 선언
	//OnComponentBeginOverlap delegate에 Bind 될 수 있음
	//Bind를 위해 reflection system 에 노출되어야 하기에 UFUNCTION
	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	

	void DebugItemNameOnScreen();
	void SetOverlappingItem(AActor* OtherActor);
	void SetOverlappingItemNull(AActor* OtherActor);
};
