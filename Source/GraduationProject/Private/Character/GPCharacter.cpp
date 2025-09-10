// C135203_GraduationProject

#include "Character/GPCharacter.h"
#include <iostream>
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Component/GPAttributeComponent.h"
#include "Component/InventorySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Component/CustomCharacterMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/GPHUD.h"
#include "HUD/PlayerHUD.h"
#include "Item/Weapon/Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Player/GPPlayerController.h"

AGPCharacter::AGPCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	//SetCapsuleHalfHeight
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.f);
	//bUseControllerRotation 설정
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationYaw = false;
	
	//자동으로 입력 방향으로 앞을 바라보게 해 준다.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	//회전 속도 지정
	GetCharacterMovement()->RotationRate = FRotator(0.f, 800.f, 0.f);
	//Set SpringArm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArm->SetupAttachment(RootComponent);
	//스프링암 길이 조정
	SpringArm->TargetArmLength = 380.f;
	SpringArm->AddWorldOffset(FVector(0.f, 0.f, 25.f));
	//Set Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	//스프링암에 Attach
	Camera->SetupAttachment(SpringArm);
	//InventorySystemComponent
	Inventory = CreateDefaultSubobject<UInventorySystemComponent>(TEXT("Inventory Component"));
	//Attribute의 OwnerGPCharacter 변수 설정
	Attribute->OwnerGPCharacter = this;
}

void AGPCharacter::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("GPCharacter"));
	
	InitOwnerController();
	InitPlayerHUD();
	//캐릭터의 레벨에 따라 Attribute Set
	Attribute->SetAttributeByLevel();
}

void AGPCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if(IsRunnig)
	{
		//Stamina 소모 및 HUD 조정
		Attribute->UseStamina(UsingStaminaRate);
		PlayerHUD->SetStaminaBarPercent(Attribute->GetStaminaPercent());
	}
	else
	{
		//Stamina 회복 및 HUD 조정
		Attribute->UseStamina(RecoveringStaminaRate);
		PlayerHUD->SetStaminaBarPercent(Attribute->GetStaminaPercent());
	}
}

//입력이 들어와 공격을 실행한다.
void AGPCharacter::Attacking()
{
	//공격 중인 상황 다음 입력이 들어온 상황.
	if(ActionState == EActionState::Attacking)
	{
		//Late하게 Combo를 이어갈 수 있는 상황(누르는 순간 다음 공격 나간다)
		if(CanComboLate == true)
		{
			DoNextComboAttack();
		}
		else
		{
			//아직 다음 콤보 공격 애니메이션이 나오기에는 이른 상황, CamComboFast를 저장해두어
			//특정 시점이 왔을 때 다음 공격 애니메이션을 재생한다. ComboNext 함수에서 실행
			SetCanComboFast();
		}
	}
	//아직 공격 중이 아닌 상황에 입력이 들어온 상황
	else
	{
		DoFirstComboAttack();
	}
}

bool AGPCharacter::IsCanAttack()
{
	//공격 가능 상태 지정
	return ActionState <= EActionState::Attacking &&
		GetCharacterState() >= ECharacterState::EquippedWeapon;
}

float AGPCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if(Attribute && PlayerHUD)
	{
		//Health 속성 조정
		Attribute->ReceiveDamage(DamageAmount);
		//HUD 의 HealthBar 위젯 조정
		PlayerHUD->SetHPBarPercent(Attribute->GetHealthPercent());
	}
	CombatTarget = EventInstigator->GetPawn();
	return DamageAmount;
}

void AGPCharacter::GetHit_Implementation(const FVector& ImpactPoint)
{
	PlayHitSound(ImpactPoint);
	SpawnHitParticle(ImpactPoint);
	//데미지 받은 후 Alive 경우
	if(Attribute && Attribute->CheckIsAlive())
	{
		//각도 별 HitReact 애니메이션 재생
		HitReactByAngle(ImpactPoint);
		SetActionState(EActionState::GettingHit);
		SetWeaponNoCollision();
		if(EquippedWeapon)
			EquippedWeapon->IgnoreActors.Empty();
	}
	//데미지 받은 후 HP 없을 경우
	else
	{
		Death();
	}
}

void AGPCharacter::PlayDodgeMontageByAngle()
{
	if(CharacterDirection >= 45.f && CharacterDirection <= 135.f)
		PlayMontageSection(DodgeMontage, FName("R"));
	else if(CharacterDirection < -135.f || CharacterDirection > 135.f)
		PlayMontageSection(DodgeMontage, FName("B"));
	else if(CharacterDirection >= -135.f && CharacterDirection <= -45.f)
		PlayMontageSection(DodgeMontage, FName("L"));
	else if(CharacterDirection > -45.f && CharacterDirection < 45.f)
		PlayMontageSection(DodgeMontage, FName("F"));
}

void AGPCharacter::SpawnLevelUpParticle()
{
	if(LevelUpParticle&&GetWorld())
	{
		
		UNiagaraFunctionLibrary::SpawnSystemAttached(LevelUpParticle, GetMesh(), FName(), FVector(), FRotator(), EAttachLocation::KeepRelativeOffset, true);
	}
}

void AGPCharacter::PlayLevelUpSound()
{
	if(LevelUpSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this,
			LevelUpSound, GetActorLocation());
	}
}

void AGPCharacter::Death()
{
	Super::Death();
	SetActionState(EActionState::Dead);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetWeaponNoCollision();
}

void AGPCharacter::AttackEnd()
{
	SetActionState(EActionState::Unoccupied);
	std::cin.clear();
}

void AGPCharacter::Arm()
{
	if(EquippedWeapon)
	{
		EquippedWeapon->AttachToSocket(GetMesh(),FName("RightHandSocket"));	
	}
}

void AGPCharacter::DisArm()
{
	if(EquippedWeapon)
	{
		EquippedWeapon->AttachToSocket(GetMesh(),FName("SpineSocket"));	
	}
}

void AGPCharacter::EndArm()
{
	SetActionState(EActionState::Unoccupied);
}

void AGPCharacter::EndDisArm()
{
	SetActionState(EActionState::Unoccupied);
}

//다음 공격 애니메이션이 실행 될 수 있는 첫 지점
void AGPCharacter::ComboNext()
{
	//이 지점이 오기 전에 미리 공격이 입력 되었을 때
	if(CanComboFast)
	{
		DoNextComboAttack();
	}
	//아직 입력이 없었지만 남은 애니메이션 시간 중에 입력이 들어오면 콤보 가능하도록 한다.
	else
	{
		CanComboLate = true;
	}
}

void AGPCharacter::ComboEnd()
{
	AttackIndex = 0;
	CanComboFast = false;
	CanComboLate = false;
}

void AGPCharacter::HitReactEnd()
{
	SetActionState(EActionState::Unoccupied);
}

void AGPCharacter::InitOwnerController()
{
	OwnerController = Cast<AGPPlayerController>(GetController());
}

void AGPCharacter::InitPlayerHUD()
{
	if(OwnerController)
	{
		GPHUD = Cast<AGPHUD>(OwnerController->GetHUD());
		if(GPHUD)
		{
			PlayerHUD = GPHUD->GetPlayerHUD();
			if(PlayerHUD && Attribute)
			{
				PlayerHUD->SetHPBarPercent(Attribute->GetHealthPercent());
				PlayerHUD->SetStaminaBarPercent(Attribute->GetStaminaPercent());
				PlayerHUD->SetSoulText(Attribute->GetSoul());
			}
		}
	}
}

void AGPCharacter::DoFirstComboAttack()
{
	CanComboFast = false;
	AttackIndex = 0;
	PlayMontageSection(AttackMontage, AttackMontageSections[AttackIndex]);
	if(AttackIndex < AttackMontageSections.Num())
		AttackIndex++;
	SetActionState(EActionState::Attacking);
}

void AGPCharacter::DoNextComboAttack()
{
	CanComboLate = false;
	PlayMontageSection(AttackMontage, AttackMontageSections[AttackIndex]);
	if(AttackIndex < AttackMontageSections.Num())
		AttackIndex++;
}

void AGPCharacter::SetCanComboFast()
{
	if(CanComboFast == false)
	{
		CanComboFast = true;
	}
}