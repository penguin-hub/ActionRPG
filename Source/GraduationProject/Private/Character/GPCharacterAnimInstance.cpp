// C135203_GraduationProject

#include "Character/GPCharacterAnimInstance.h"
#include "KismetAnimationLibrary.h"
#include "Character/GPCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UGPCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	SetGPCharacter();
	SetGPCharacterMovement();
}

void UGPCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	UpdateCharacterSpeed();
	UpdateCharacterDirection();
	UpdateIsFalling();
	UpdateCharacterState();
	UpdateActionState();
	UpdateDeathPose();
	UpdateIsFocus();
}

void UGPCharacterAnimInstance::SetGPCharacter()
{
	GPCharacter = Cast<AGPCharacter>(TryGetPawnOwner());
}

void UGPCharacterAnimInstance::SetGPCharacterMovement()
{
	if(GPCharacter)
	{
		//Owner GPCharacter의 무브먼트를 변수에 저장
		GPCharacterMovement = GPCharacter->GetCharacterMovement();
	}
}

void UGPCharacterAnimInstance::UpdateCharacterSpeed()
{
	if(GPCharacterMovement)
	{
		//캐릭터의 스피드를 저장, 땅에서의 걷기와 뛰기 등에 사용할 변수이기에 XY축만 고려
		CharacterSpeed = UKismetMathLibrary::VSizeXY(GPCharacterMovement->Velocity);
	}
}

void UGPCharacterAnimInstance::UpdateCharacterDirection()
{
	if(GPCharacter && GPCharacterMovement)
		CharacterDirection = UKismetAnimationLibrary::CalculateDirection(GPCharacterMovement->Velocity, GPCharacter->GetActorRotation());
}

void UGPCharacterAnimInstance::UpdateIsFalling()
{
	if(GPCharacterMovement)
		IsFalling = GPCharacterMovement->IsFalling();
}

void UGPCharacterAnimInstance::UpdateCharacterState()
{
	if(GPCharacter)
		CharacterState = GPCharacter->GetCharacterState();
}

void UGPCharacterAnimInstance::UpdateActionState()
{
	if(GPCharacter)
		ActionState = GPCharacter->GetActionState();
}

void UGPCharacterAnimInstance::UpdateDeathPose()
{
	if(GPCharacter)
		DeathPose = GPCharacter->GetDeathPose();
}

void UGPCharacterAnimInstance::UpdateIsFocus()
{
	if(GPCharacter)
		IsFocus = GPCharacter->GetIsFocus();
}