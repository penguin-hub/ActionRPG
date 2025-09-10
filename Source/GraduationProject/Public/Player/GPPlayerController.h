// C135203_GraduationProject
#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "GPPlayerController.generated.h"

class AGPCharacter;
class UInputMappingContext;
class UInputAction;
class UAnimMontage;
struct FInputActionValue;
/**
 * 
 */
UCLASS()
class GRADUATIONPROJECT_API AGPPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AGPPlayerController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY()
	TObjectPtr<AGPCharacter> HavingGPCharacter;
private:
	UPROPERTY(EditDefaultsOnly, Category= Montage)
	UAnimMontage* ArmDisArmMontage;
	//락온 위한 타겟 액터
	UPROPERTY()
	AActor* ActorToLockOn;
	
	/**
	 *입력
	 */
	//입력 매핑 컨텍스트 지정
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> GPContext;
	//캐릭터 이동 인풋 액션 지정
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	//카메라 이동 인풋 액션 지정
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> Look_Mouse_Action;
	//캐릭터 점프 인풋 액션 지정
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> Jump_Action;
	//캐릭터 달리기 인풋 액션 지정
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> Run_Action;
	//장비 인풋 액션 지정
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> InterAct_Action;
	//공격 인풋 액션 지정
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> Attack_Action;
	//록온 인풋 액션 지정
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> LockOn_Action;
	//Crouch,앉기 인풋 액션 지정
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> Crouch_Action;
	//인벤토리 인풋 액션 지정
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> InventoryMenu_Action;
	//EquipmentMenu 인풋 액션 지정
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> EquipmentMenu_Action;
	//Climbing 인풋 액션 지정
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> Climbing_Action;
	
	//캐릭터 이동 인풋 액션에 바인딩 하기 위한 함수
	void Move(const FInputActionValue& InputActionValue);
	//카메라 이동 인풋 액션에 바인딩 하기 위한 함수
	void Look_Mouse(const FInputActionValue& InputActionValue);
	//점프 인풋 액션에 바인딩 하기 위한 함수
	void Jump(const FInputActionValue& InputActionValue);
	//달리기 인풋 액션에 바인딩 하기 위한 함수
	void RunStart(const FInputActionValue& InputActionValue);
	void RunEnd(const FInputActionValue& InputActionValue);
	//상호작용 인풋 액션에 바인딩 하기 위한 함수
	void InterAct(const FInputActionValue& InputActionValue);
	//공격 인풋 액션에 바인딩 하기 위한 함수
	void Attack(const FInputActionValue& InputActionValue);
	//인벤토리 인풋 액션에 바인딩 하기 위한 함수
	void Lock_On(const FInputActionValue& InputActionValue);
	void Lock_On_End(const FInputActionValue& InputActionValue);
	//Crouch,앉기 인풋 액션에 바인딩 하기 위한 함수
	void Crouch(const FInputActionValue& InputActionValue);
	//인벤토리 인풋 액션에 바인딩 하기 위한 함수
	void InventoryMenu(const FInputActionValue& InputActionValue);
	//EquipmentMenu 인풋 액션에 바인딩 하기 위한 함수
	void EquipmentMenu(const FInputActionValue& InputActionValue);
	//Climbing 인풋 액션에 바인딩 하기 위한 함수
	void Climbing(const FInputActionValue& InputActionValue);
	
	void PlayMontageSection(UAnimMontage* Montage,const FName& SectionName);
	virtual void SetupInputComponent() override;
	void AddMappingContextToController(UInputMappingContext* InputMappingContext) const;
	void BindingFunctionToInputAction();
	void MoveCharacterByInputAxisVector(FVector2d InputAxisVector);
	void RotateControllerByInputAxisVector(FVector2d InputAxisVector);
	void SetHavingGPCharacter();
	bool CanInteract();
	bool CanArm();
	bool CanDisArm();
	bool CanJump();
	void CameraToActorToLockOn();
	bool CanNotMove();
	bool CanRun();
	bool CanStopRun();
	void DoSphereTraceToFindLockOnTarget(FHitResult& SphereHitResult);
	void SphereTraceToCheckCanUncrouch(FHitResult& SphereHitResult);
	void DoCrouch();
	void DoUncrouch();
};





