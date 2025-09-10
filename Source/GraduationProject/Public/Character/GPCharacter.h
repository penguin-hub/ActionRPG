// C135203_GraduationProject
#pragma once

#include "CoreMinimal.h"
#include "CharacterType.h"
#include "Character/GPCharacterBase.h"
#include "GPCharacter.generated.h"

class UCustomCharacterMovementComponent;
class UNiagaraSystem;
class AGPHUD;
class AGPPlayerController;
class UInventorySystemComponent;
class AItem;
class USpringArmComponent;
class UCameraComponent;
class AGPCharacter;
class UPlayerHUD;

/**
 * 
 */
UCLASS()
class GRADUATIONPROJECT_API AGPCharacter : public AGPCharacterBase
{
	GENERATED_BODY()
	
public:
	AGPCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	//카메라
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;
	//스프링암
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;
	//인벤토리시스템 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInventorySystemComponent* Inventory;
	//게임모드의 기본 HUD
	UPROPERTY()
	AGPHUD* GPHUD;
	//플레이어에게 보여지는 기본 HUD
	UPROPERTY()
	UPlayerHUD* PlayerHUD;
	//overlap 된 아이템을 저장하기 위한 변수
	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;
	//달리는 중인지 확인 위한 변수
	bool IsRunnig = false;
	//RunningAttack 몽타주
	UPROPERTY(EditDefaultsOnly, Category= Montage)
	UAnimMontage* RunningAttack;
	
	
	//공격 실행 함수
	virtual void Attacking() override;
	//공격 가능 상태인지 확인
	virtual bool IsCanAttack() override;
	//ApplyDamage 에 따른 TakeDamage 함수
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	//IHitInterface로부터 상속 받은 함수, 공격을 맞았을 때 실행
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	//DodgeMontage 를 재생
	void PlayDodgeMontageByAngle();
	//LevelUp Effects
	void SpawnLevelUpParticle();
	void PlayLevelUpSound();
	//Get Set 함수, 간단한 함수이므로 INLINE으로 선언
	FORCEINLINE void SetOverlappingItem(AItem* Item) {OverlappingItem = Item;}
	FORCEINLINE ECharacterState GetCharacterState() const  {return CharacterState;}
	FORCEINLINE void SetCharacterState(const ECharacterState State) {CharacterState = State;}
	FORCEINLINE EActionState GetActionState() const {return ActionState;}
	FORCEINLINE void SetActionState(const EActionState State) {ActionState = State;}
	FORCEINLINE bool GetIsFocus() const {return IsFocus;}
	FORCEINLINE void SetIsFocus(const bool boolean) {IsFocus = boolean;}\
	
protected:
	//Run 시 매 틱 마다 소모 되는 Stamina
	UPROPERTY(EditDefaultsOnly)
	float UsingStaminaRate = 0.2f;
	//매 틱 마다 회복 되는 Stamina
	UPROPERTY(EditDefaultsOnly)
	float RecoveringStaminaRate = -0.2f;
	//Focus 움직임 도중 회피 점프 몽타주
	UPROPERTY(EditDefaultsOnly, Category= Montage)
	UAnimMontage* DodgeMontage;
	
	//HP가 0 이하가 되었을 때 실행, Death 함수
	virtual void Death() override;
	/*
	 *애니메이션 노티파이 연결 위한 함수들
	 */
	//공격이 끝나고 난 후 실행
	virtual void AttackEnd() override;
	//무기장착 때에 손과 무기 닿는 순간 실행, RightHandSocket에 무기 attach
	UFUNCTION(BlueprintCallable)
	void Arm();
	//무장 해제, 무기 등에 장착 때 무기 등에 닿는 순간 실행
	UFUNCTION(BlueprintCallable)
	void DisArm();
	//Arm애니메이션 끝날 때, ActionState전환
	UFUNCTION(BlueprintCallable)
	void EndArm();
	//DisArm애니메이션 끝날 때, ActionState전환
	UFUNCTION(BlueprintCallable)
	void EndDisArm();
	//하나의 공격 실행 후에 다음 공격 넘어갈 수 있는 첫 타이밍, 이 시점 이전에 입력 있었을 경우 다음 공격 실행
	UFUNCTION(BlueprintCallable)
	void ComboNext();
	//ComboEnd 시점 까지 기다렸지만 입력이 없어 다음 공격 으로 넘어가지 않고 콤보 공격 종료
	UFUNCTION(BlueprintCallable)
	void ComboEnd();
	//HitReact 끝날 때 실행, ActionState 전환
	UFUNCTION(BlueprintCallable)
	void HitReactEnd();
	/*
	 *
	 */
private:
	//AttackMontageSections 중 현재 공격 index
	int32 AttackIndex = 0;
	bool IsAttacking = false;
	//CanComboFast 일 때 ComboNext 시점에서 다음 공격이 나간다.
	bool CanComboFast = false;
	//CanComboLate 는 ComboNext~ComboEnd 구간에서 true, 입력이 들어오면 즉시 다음 공격 실행
	bool CanComboLate =false;
	//캐릭터가 집중 상태, 이 때에는 캐릭터의 방향과 회전 설정이 바뀐다. ActorToLockOn 이 있을 경우 해당 액터를 바라봄
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	bool IsFocus = false;
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* LevelUpParticle;
	UPROPERTY(EditAnywhere)
	USoundBase* LevelUpSound;
	UPROPERTY()
	AGPPlayerController* OwnerController;
	//캐릭터의 상태를 나타내기 위한 변수
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	ECharacterState CharacterState = ECharacterState::Unequipped;
	//캐릭터의 액션 상태를 나타내기 위한 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	EActionState ActionState = EActionState::Unoccupied;

	void InitOwnerController();
	void InitPlayerHUD();
	void DoFirstComboAttack();
	void DoNextComboAttack();
	void SetCanComboFast();
	
};
