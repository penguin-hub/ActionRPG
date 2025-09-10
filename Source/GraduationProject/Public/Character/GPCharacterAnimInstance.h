// C135203_GraduationProject
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Character/CharacterType.h"
#include "GPCharacterAnimInstance.generated.h"
/**
 * 
 */
UCLASS()
class GRADUATIONPROJECT_API UGPCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly)
	class AGPCharacter* GPCharacter;
	UPROPERTY(BlueprintReadOnly)
	class UCharacterMovementComponent* GPCharacterMovement;
	//캐릭터 스피드 저장
	UPROPERTY(BlueprintReadOnly)
	float CharacterSpeed;
	//캐릭터 방향 저장
	UPROPERTY(BlueprintReadOnly)
	float CharacterDirection;
	//공중 떠 있는지 확인 가능
	UPROPERTY(BlueprintReadOnly)
	bool IsFalling;
	UPROPERTY(BlueprintReadOnly)
	ECharacterState CharacterState;
	UPROPERTY(BlueprintReadOnly)
	EActionState ActionState;
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;
	UPROPERTY(BlueprintReadOnly)
	bool IsFocus;
private:
	void SetGPCharacter();
	void SetGPCharacterMovement();
	void UpdateCharacterSpeed();
	void UpdateCharacterDirection();
	void UpdateIsFalling();
	void UpdateCharacterState();
	void UpdateActionState();
	void UpdateDeathPose();
	void UpdateIsFocus();
};
