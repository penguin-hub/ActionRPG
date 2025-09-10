// C135203_GraduationProject
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/HitInterface.h"
#include "GPCharacterBase.generated.h"

enum class EDeathPose: uint8;
class UGPAttributeComponent;
class AWeapon;

UCLASS(Abstract)
class GRADUATIONPROJECT_API AGPCharacterBase : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AGPCharacterBase(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	//캐릭터의 Attribute 를 포함하고 있는 Attribute 컴포넌트
	UPROPERTY(VisibleAnywhere)
	UGPAttributeComponent* Attribute;
	//달리기 속도를 위한 변수, 달리기 시 캐릭터 스피드*RunSpeed
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float RunSpeed = 1.5f;
	//캐릭터의 방향, Velocity 와 Forward Vector 사이의 각도
	UPROPERTY(BlueprintReadOnly)
	float CharacterDirection;
	//장착중인 Weapon을 가리키는 변수
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	AWeapon* EquippedWeapon;
	//Get, Set
	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const {return DeathPose;}
	FORCEINLINE AActor* GetCombatTarget() const {return CombatTarget;}
	FORCEINLINE void SetCombatTarget(AActor* Target) {CombatTarget = Target;}
	FORCEINLINE UCustomCharacterMovementComponent* GetCustomCharacterMovementComponent() const {return CustomCharacterMovementComponent;}
protected:
	UPROPERTY(VisibleAnywhere)
	UCustomCharacterMovementComponent* CustomCharacterMovementComponent;
	UPROPERTY(EditAnywhere)
	USoundBase* GetHitSound;
	UPROPERTY(EditAnywhere)
	UParticleSystem* HitParticle;
	//애니메이션 몽타주들
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* DeathMontage;
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* HitReactMontage;
	//몽타주 섹션들의 배열
	UPROPERTY(EditAnywhere)
	TArray<FName> AttackMontageSections;
	UPROPERTY(EditAnywhere)
	TArray<FName> DeathMontageSections;
	//전투 상대
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* CombatTarget;
	//DeathPose 저장, Death 애니메이션 재생 후 해당 포즈 재생
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TEnumAsByte<EDeathPose> DeathPose;
	
	virtual void Attacking();
	virtual bool IsCanAttack();
	virtual void Death();
	//해당 애니메이션 몽타주의 해당 섹션 재생
	void PlayMontageSection(UAnimMontage* Montage,const FName& SectionName);
	//해당 섹션 배열 중 랜덤 섹션 재생
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);
	int32 PlayAttackMontage();
	void StopAttackMontage();
	//DeathMontage 랜덤 재생 및 DeathPose 저장
	virtual int32 PlayDeathMontage();
	void PlayHitReactMontage(const FName& SectionName);
	//Hit각도별 HitReact 재생
	void HitReactByAngle(const FVector& ImpactPoint);
	void DebugHitAngle(FVector Forward, FVector ToHit, double Theta, FVector CrossProduct);
	//Hit각도 계산, 
	double HitAngleCalculation(const FVector& ImpactPoint);
	/*
	 *애니메이션 노티파이 연결 위한 함수들
	 */
	//공격이 끝나고 난 후 실행할 일들을 위한 함수, 애니메이션 노티파이에 연결 위한
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	//공격 애니메이션중에 무기의 Collision 모드 전환, BoxTrace 가능불가능 위해, 무기의 IgnoreActors초기화
	UFUNCTION(BlueprintCallable)
	void EnableDisableWeaponCollision(ECollisionEnabled::Type CollisionEnabled);
	/*
	 *
	 */
	//MotionWarping중 사용
	UPROPERTY(EditAnywhere)
	double WarpDistance = 90.f;
	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();
	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();
	//gethit 에서
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticle(const FVector& ImpactPoint);
	void SetCharacterDirection();
	void SetWeaponNoCollision();
};



