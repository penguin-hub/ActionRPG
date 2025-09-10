// C135203_GraduationProject
#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "Weapon.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class GRADUATIONPROJECT_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<AActor*> IgnoreActors;
	
	void AttachToSocket(USceneComponent* InParent, FName InSocketName);
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);
	//IItemInteractInterface함수
	virtual void ItemInteract(AActor*) override;
	
	FORCEINLINE UBoxComponent* GetBox() const {return Box;}
protected:
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void DoBoxTrace(FHitResult& BoxHitResult);
	//c++에서 실행하지만 나머지 작업들은 블루프린트에서
	UFUNCTION(BlueprintImplementableEvent)
	void CreateField(const FVector& FieldLocation);
private:
	//공격 Collision을 위한 박스 컴포넌트
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Box;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* TraceStart;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* TraceEnd;
	UPROPERTY(EditAnywhere)
	float Damage;

	void SetWeaponDamage();
	void PlayEquipSound(AActor* NewOwner);
};
