// C135203_GraduationProject
#pragma once

#include "CoreMinimal.h"
#include "Character/GPCharacterBase.h"
#include "Character/CharacterType.h"
#include "Engine/DataTable.h"
#include "GPEnemy.generated.h"

class UPawnSensingComponent;
class UHealthBarComponent;
/**
 * 
 */
UCLASS()
class GRADUATIONPROJECT_API AGPEnemy : public AGPCharacterBase
{
	GENERATED_BODY()
public:
	AGPEnemy(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	//게임플레이 동안 destroyed 일 때만 발동, 게임플레이 끝날 때 발동되지 않는다.
	virtual void Destroyed() override;

	void SetAttributeByEnemyData();
	//공격 실행 함수
	virtual void Attacking() override;
	//IHitInterface로부터 상속 받은 함수, 공격을 맞았을 때 실행
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	//ApplyDamage 에 따라 TakeDamage 함수 실행
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void GiveXPToCharacter();
	//HP가 0이 되었을 때 수행할 일을 담은 함수
	virtual void Death() override;
	FORCEINLINE EEnemyState GetEnemyState() const  {return EnemyState;}
	FORCEINLINE void SetEnemyState(const EEnemyState State) {EnemyState = State;}
protected:
	//EnemyState
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EEnemyState EnemyState = EEnemyState::Patrolling;

	//AttackEnd 노티파이와 연결, 공격 애니메이션이 끝날 때 실행
	virtual void AttackEnd() override;
	//공격 가능 상태인지 확인
	virtual bool IsCanAttack() override;
	//FSeePawnDelegat 에 바인딩 하기 위한 함수, 적 발견 시 실행
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);
	//캐릭터와 Target의 거리가 Radius 안에 있는지 체크
	bool IsTargetInRadius(AActor* Target, double Radius);
	//자동으로 길을 검색해 Target에게 이동
	void MoveToTarget(AActor* Target);
	//MoveToTarget의 경로 Debug 버전
	void MoveToTargetDebugVersion(AActor* Target);
	//PatrolTargets 배열 에서 다음 PatrolTarget 선택
	AActor* ChoosePatrolTarget();
private:
	//Controller
	UPROPERTY()
	class AAIController* EnemyController;
	UPROPERTY(EditAnywhere)
	FDataTableRowHandle EnemyDataTableRowHandle;
	float GiveXP;
	//RightHand 장착할 무기 클래스의 UClass
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;
	//캐릭터 속도 관련 변수
	UPROPERTY(EditAnywhere, Category= Combat)
	float PatrollingSpeed = 125.f;
	UPROPERTY(EditAnywhere, Category= Combat)
	float ChasingSpeed = 300.f;
	//PawnSensing을 위한 Components
	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensingComponent;
	//HealthBar표시를 위한 위젯컴포넌트, HealthBar 위젯을 가짐
	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarComponent;
	//전투 범위, 이 범위 안에 들어오면 쫓아간다.
	UPROPERTY(EditAnywhere)
	double CombatRadius = 600.f;
	//공격 범위, 이 범위 안에 들어오면 공격을 시도한다.
	UPROPERTY(EditAnywhere)
	double AttackRadius = 200.f;
	//PatrolTarget, 현재 이동 목표 지점
	UPROPERTY(EditInstanceOnly)
	AActor* PatrolTarget;
	//PatrolTarget들의 배열, 예비 이동 목표 지점들의 집합
	UPROPERTY(EditInstanceOnly)
	TArray<AActor*> PatrolTargets;
	//PatrolTarget에 도착했다고 인식하는 범위
	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;
	//PatrolTarget 도착 후 대기 시간을 위한 타이머
	FTimerHandle PatrolTimer;
	//PatrolTimer 랜덤 시간을 위한 최소 시간
	UPROPERTY(EditAnywhere)
	float PatrolWaitMin = 2.f;
	//PatrolTimer 랜덤 시간을 위한 최대 시간
	UPROPERTY(EditAnywhere)
	float PatrolWaitMax = 5.f;
	//AttackTimer를 위한 FTimerHandle
	FTimerHandle AttackTimer;
	//AttackTimer의 랜덤 시간을 위한 최소 시간과 최대 시간 
	UPROPERTY(EditAnywhere, Category= Combat)
	float AttackMin = 0.3f;
	UPROPERTY(EditAnywhere, Category= Combat)
	float AttackMax = 0.7f;
	
	//PatrolTimer 후에 실행될 함수
	void PatrolTimerFinished();
	//CombatTarget에게 Interest 사라짐
	void NoInterest();
	void ChaseCombatTarget();
	bool ShouldChase();
	bool ShouldAttack();
	//PatrolTarget 과의 거리 체크 후 해야 할 일 수행
	void CheckPatrolTarget();
	//CombatTarget 과의 거리 체크 후 해야 할 일 수행
	void CheckCombatTarget();
	//Timer를 맞추며 공격 시작
	void StartAttackTimer();
	void ClearAttackTimer();
	void StartPatrolTimer();
	void ClearPatrolTimer();
	void SpawnWeaponClassAndEquip();
	void SetHealthBarVisibility(bool Visibility);
};
