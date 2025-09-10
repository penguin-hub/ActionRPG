// C135203_GraduationProject

#include "Character/GPCharacterBase.h"
#include "KismetAnimationLibrary.h"
#include "Character/CharacterType.h"
#include "Component/CustomCharacterMovementComponent.h"
#include "Component/GPAttributeComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Item/Weapon/Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AGPCharacterBase::AGPCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	PrimaryActorTick.bCanEverTick = false;
	//Set Attribute 컴포넌트
	Attribute = CreateDefaultSubobject<UGPAttributeComponent>(TEXT("Attribute"));
	GetMesh()->SetGenerateOverlapEvents(true);

	CustomCharacterMovementComponent = Cast<UCustomCharacterMovementComponent>(GetCharacterMovement());
}

void AGPCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AGPCharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	SetCharacterDirection();
}

void AGPCharacterBase::Attacking()
{
	//상대가 "Die" 태그 가지고 있을 시 CombatTraget 으로 하지 않음
	if(CombatTarget && CombatTarget->ActorHasTag(FName("Die")))
	{
		CombatTarget = nullptr;
	}
}

bool AGPCharacterBase::IsCanAttack()
{
	return false;
}

void AGPCharacterBase::Death()
{
	Tags.Add(FName("Die"));
	PlayDeathMontage();
}

void AGPCharacterBase::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

int32 AGPCharacterBase::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	if(SectionNames.Num() > 0)
	{
		const int32 MaxSectionIndex = SectionNames.Num() - 1;
		const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
		PlayMontageSection(Montage, SectionNames[Selection]);
		return Selection;
	}
	return -1;
}

int32 AGPCharacterBase::PlayAttackMontage()
{
	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

void AGPCharacterBase::StopAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance)
	{
		AnimInstance->Montage_Stop(0.5f, AttackMontage);
	}
}

int32 AGPCharacterBase::PlayDeathMontage()
{
	const int32 Selection =  PlayRandomMontageSection(DeathMontage, DeathMontageSections);
	TEnumAsByte<EDeathPose> Pose(Selection);
	if(Pose < EDeathPose::Max)
	{
		DeathPose = Pose;
	}
	return Selection;
}

void AGPCharacterBase::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void AGPCharacterBase::HitReactByAngle(const FVector& ImpactPoint)
{
	double Theta = HitAngleCalculation(ImpactPoint);
	FName Section("HitBack");
	if(Theta >= -45.f && Theta < 45.f)
		Section = FName("HitFront");
	else if (Theta >= -135.f && Theta < -45.f)
		Section = FName("HitLeft");
	else if (Theta >= 45.f && Theta < 135.f)
		Section = FName("HitRight");
	PlayHitReactMontage(Section);
}

double AGPCharacterBase::HitAngleCalculation(const FVector& ImpactPoint)
{
	//캐릭터의 ForwardVector, GetActorForwardVector는 단위벡터 리턴
	const FVector Forward = GetActorForwardVector();
	//임팩트 포인트 벡터의 Z 값 조정한 값
	const FVector ImpactPointAdjustedZ(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	//캐릭터~Hit된 포인트(Z값 동일하게 조정) 벡터, GetSafeNormal로 노말라이즈(0으로 나누는거에 대해서 Safe)
	const FVector ToHit = (ImpactPointAdjustedZ - GetActorLocation()).GetSafeNormal();
	
	//Forward*ToHit = |Forward||ToHit| * cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	//Acos 활용 각도 구한다, 해당 각도는 라디안 각도
	double Theta = FMath::Acos(CosTheta);
	//Radian To Degree
	Theta = FMath::RadiansToDegrees(Theta);

	//언리얼엔진 외적에 대해 왼손법칙 적용, 외적 결과가 음수 이면 각도가 음수
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if(CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}
	
	//DebugHitAngle(Forward, ToHit, Theta, CrossProduct);

	return Theta;
}

void AGPCharacterBase::DebugHitAngle(const FVector Forward, const FVector ToHit, double Theta, const FVector CrossProduct)
{
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 60.f, 5.f, FColor::Green, 5.f);
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
	}
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);
}

void AGPCharacterBase::AttackEnd()
{
}

void AGPCharacterBase::EnableDisableWeaponCollision(ECollisionEnabled::Type CollisionEnabled)
{
	if(EquippedWeapon && EquippedWeapon->GetBox())
	{
		EquippedWeapon->GetBox()->SetCollisionEnabled(CollisionEnabled);
		//공격 중 이미 한 번 Hit 하여 Ignore하기로 한 Hit했던 액터들 초기화
		EquippedWeapon->IgnoreActors.Empty();
	}
}

FVector AGPCharacterBase::GetTranslationWarpTarget()
{
	if(CombatTarget)
	{
		const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
		const FVector ActorLocation = GetActorLocation();
		//CombatTarget 에서 나까지의 단위 벡터
		FVector TargetToMe = (ActorLocation - CombatTargetLocation).GetSafeNormal();
		TargetToMe *= WarpDistance;
		//DrawDebugSphere(GetWorld(), CombatTargetLocation + TargetToMe, 15.f, 12, FColor::Yellow, true);
		return CombatTargetLocation + TargetToMe;
	}
	
	return FVector();
}

FVector AGPCharacterBase::GetRotationWarpTarget()
{
	if(CombatTarget)
	{
		return CombatTarget->GetActorLocation();
	}
	
	return FVector();
}

void AGPCharacterBase::PlayHitSound(const FVector& ImpactPoint)
{
	if(GetHitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this,
			GetHitSound, ImpactPoint);
	}
}

void AGPCharacterBase::SpawnHitParticle(const FVector& ImpactPoint)
{
	if(HitParticle&&GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
			HitParticle, ImpactPoint);
	}
}

void AGPCharacterBase::SetCharacterDirection()
{
	CharacterDirection = UKismetAnimationLibrary::CalculateDirection(GetCharacterMovement()->Velocity, GetActorRotation());
}

void AGPCharacterBase::SetWeaponNoCollision()
{
	if(EquippedWeapon)
		EquippedWeapon->GetBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
