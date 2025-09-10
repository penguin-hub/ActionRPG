// C135203_GraduationProject

#include "Character/GPEnemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "AIController.h"
#include "Character/EnemyStruct.h"
#include "Character/GPCharacter.h"
#include "Component/GPAttributeComponent.h" 
#include "Game/GPGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/HealthBarComponent.h"
#include "HUD/PlayerHUD.h"
#include "Item/Weapon/Weapon.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Player/GPPlayerController.h"

AGPEnemy::AGPEnemy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	//Mesh Set
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	//Capsule Set
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	//Rotation 관련 Set
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	//HealthBarComponent Set
	HealthBarComponent = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarComponent->SetupAttachment(GetRootComponent());
	//PawnSensingComponent Set
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensingComponent->SightRadius = 45.f;
	PawnSensingComponent->SetPeripheralVisionAngle(45.f);
}

void AGPEnemy::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("GPEnemy"));
	EnemyController = Cast<AAIController>(GetController());

	SetAttributeByEnemyData();
	SpawnWeaponClassAndEquip();
	
	//체력바 처음에는 보이지 않게 설정
	SetHealthBarVisibility(false);
	
	//PawnSensing OnSeePawn 함수 바인딩
	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AGPEnemy::PawnSeen);
	}

	//PatrolTarget으로 이동
	//MoveToTargetDebugVersion(PatrolTarget);
	MoveToTarget(PatrolTarget);
}

void AGPEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if(EnemyState == EEnemyState::Dead)
		return;
	//Patrolling 보다 높은 상태, 즉 전투에 더 가까운 상태
	if(EnemyState > EEnemyState::Patrolling)
	{
		//CombatTarget과의 거리에 따라 해야할 일을 행동
		CheckCombatTarget();
	}
	//EEnmyState == Patrolling
	else
	{
		//PatrolTarget과의 거리에 따라 해야할 일을 행동
		CheckPatrolTarget();
	}
}

void AGPEnemy::Destroyed()
{
	Super::Destroyed();
	
	if(EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
}

void AGPEnemy::SetAttributeByEnemyData()
{
	UGPGameInstance* GPGameInstance = Cast<UGPGameInstance>(GetGameInstance());
	if(GPGameInstance)
	{
		UDataTable* EnemyData = GPGameInstance->GetEnemyDataTable();
		if(EnemyData)
		{
			FEnemyStruct* EnemyStruct = EnemyData->FindRow<FEnemyStruct>(EnemyDataTableRowHandle.RowName, "");
			if(EnemyStruct)
			{
				Attribute->SetMaxHealth(EnemyStruct->MaxHealth);
				Attribute->SetHealth(EnemyStruct->MaxHealth);
				GiveXP  = EnemyStruct->GiveXP;
			}
		}
	}
}

void AGPEnemy::Attacking()
{
	Super::Attacking();
	
	if(CombatTarget == nullptr)
		return;
	SetEnemyState(EEnemyState::Engaged);
	PlayAttackMontage();
}

void AGPEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	PlayHitSound(ImpactPoint);
	SpawnHitParticle(ImpactPoint);
	ClearPatrolTimer();
	ClearAttackTimer();
	SetWeaponNoCollision();
	StopAttackMontage();
	//데미지 받은 후 살아있을 경우
	if(Attribute && Attribute->CheckIsAlive())
	{
		SetHealthBarVisibility(true);
		HitReactByAngle(ImpactPoint);
		if(EquippedWeapon)
			EquippedWeapon->IgnoreActors.Empty();
	}
	//데미지 받은 후 HP 없을 경우
	else
	{
		Death();
	}
}

float AGPEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if(Attribute&&HealthBarComponent)
	{
		//받은 Damage 만큼 Health 조정
		Attribute->ReceiveDamage(DamageAmount);
		//Damage 받은 만큼 HealthBar UI 조정
		HealthBarComponent->SetHealthPercent(Attribute->GetHealthPercent()); 
	}

	ClearPatrolTimer();
	//공격 상대 CombatTarget으로 설정
	CombatTarget = EventInstigator->GetPawn();
	//CombatTarget AttackRadius 안에 있을 경우
	if(IsTargetInRadius(CombatTarget, AttackRadius))
	{
		SetEnemyState(EEnemyState::Chasing);
	}
	//CombatTarget CombatRadius 안에 있을 경우
	else if(IsTargetInRadius(CombatTarget, CombatRadius))
	{
		ChaseCombatTarget();
	}
	
	return DamageAmount;
}

void AGPEnemy::GiveXPToCharacter()
{
	AGPPlayerController* GPController = Cast<AGPPlayerController>(GetWorld()->GetFirstPlayerController());
	if(GPController)
	{
		AGPCharacter* Character = Cast<AGPCharacter>(GPController->GetCharacter());
		if(Character)
		{
			Character->Attribute->AddXP(GiveXP);
			Character->PlayerHUD->SetCurrentXPText(Character->Attribute->GetCurrentXP());
		}
	}
}

void AGPEnemy::Death()
{
	Super::Death();
	if(CombatTarget)
	{
		//플레이어 캐릭터의 CombatTarget 에서도 제거
		AGPCharacter* GPCharacter = Cast<AGPCharacter>(CombatTarget);
		if(GPCharacter)
		{
			GPCharacter->SetCombatTarget(nullptr);
		}
		CombatTarget = nullptr;
	}
		
	SetEnemyState(EEnemyState::Dead);
	//Tick 정지
	SetActorTickEnabled(false);
	ClearAttackTimer();
	//캐릭터에게 경험치 GiveXP 만큼 준다.
	GiveXPToCharacter();
	//체력바 안 보이게 한다.
	SetHealthBarVisibility(false);
	//콜리전 해제
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetWeaponNoCollision();
	//일정 시간 후에 적 캐릭터 사라지게 한다.
	SetLifeSpan(4.f);

	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void AGPEnemy::AttackEnd()
{
	Super::AttackEnd();
	//먼저 NoState로 변경 후에 상황 Check해 적절한 State로 변경
	//공격이 끝난 후 플레이어캐릭터의 움직임에 따라 상황이 다르기 때문
	SetEnemyState(EEnemyState::NoState);
	CheckCombatTarget();
}

bool AGPEnemy::IsCanAttack()
{
	bool bCanAttack = IsTargetInRadius(CombatTarget, AttackRadius) && EnemyState != EEnemyState::Attacking
		&& EnemyState != EEnemyState::Dead;
	return bCanAttack;
}

void AGPEnemy::PawnSeen(APawn* SeenPawn)
{
	if(EnemyState > EEnemyState::Patrolling) return;
	if(SeenPawn->ActorHasTag(FName("GPCharacter")))
	{
		ClearPatrolTimer();
		CombatTarget = SeenPawn;
		ChaseCombatTarget();
		UE_LOG(LogTemp, Warning, TEXT("See, Start Chasing"));
		//상대 캐릭터의 CombatTarget this 로 설정
		if(AGPCharacter* GPCharacter = Cast<AGPCharacter>(SeenPawn))
		{

			GPCharacter->SetCombatTarget(this);
		}
	}
}

bool AGPEnemy::IsTargetInRadius(AActor* Target, double Radius)
{
	if(Target == nullptr)
		return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	//DrawDebugSphere(GetWorld(), GetActorLocation(), 12.f, 12, FColor::Yellow, false, 1.f);
	//DrawDebugSphere(GetWorld(), Target->GetActorLocation(), 12.f, 12, FColor::Red, false, 1.f);
	return DistanceToTarget <= Radius;
}

void AGPEnemy::MoveToTarget(AActor* Target)
{
	if(EnemyController == nullptr || Target == nullptr)
		return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(30.f);
	//해당 지점으로 길을 찾아 이동
	EnemyController->MoveTo(MoveRequest);
}

void AGPEnemy::MoveToTargetDebugVersion(AActor* Target)
{
	if(EnemyController && Target)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(30.f);
		//PathPoints 보여줌
		FNavPathSharedPtr NavigationPath;
		EnemyController->MoveTo(MoveRequest, &NavigationPath);
		TArray<FNavPathPoint>& PathPoints = NavigationPath->GetPathPoints();
		for (auto& Point : PathPoints)
		{
			const FVector& Location = Point.Location;
			DrawDebugSphere(GetWorld(), Location, 12.f, 12, FColor::Green, false, 10.f);
		}
	}
}

AActor* AGPEnemy::ChoosePatrolTarget()
{
	//현재 타겟과 같은 타겟 제외하고 검색하기 위한 배열
	TArray<AActor*> ValidTargets;
	for(auto Targetmem : PatrolTargets)
	{
		if(Targetmem != PatrolTarget)
		{
			//PatrolTarget과 같지 않을 때 ValidTarget 에 추가
			ValidTargets.AddUnique(Targetmem);
		}
	}
	const int32 PatrolTargetsNum = ValidTargets.Num();
	if(PatrolTargetsNum > 0)
	{
		//랜덤으로 ValidTargets 중 하나 선택
		const int32 TargetSelection = FMath::RandRange(0, PatrolTargetsNum - 1);
		AActor* Target = ValidTargets[TargetSelection];
		return Target;
	}
	return nullptr;
}

void AGPEnemy::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}

void AGPEnemy::NoInterest()
{
	UE_LOG(LogTemp, Warning, TEXT("No Interest"));
	SetHealthBarVisibility(false);
	//patrolling 다시 시작
	if(EnemyState != EEnemyState::Engaged)
	{
		SetEnemyState(EEnemyState::Patrolling);
		GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
		MoveToTarget(PatrolTarget);
	}
	//플레이어 캐릭터의 CombatTarget 에서도 제거
	AGPCharacter* GPCharacter = Cast<AGPCharacter>(CombatTarget);
	if(GPCharacter)
	{
		GPCharacter->SetCombatTarget(nullptr);
	}
	CombatTarget = nullptr;
}

void AGPEnemy::ChaseCombatTarget()
{
	UE_LOG(LogTemp, Warning, TEXT("Chasing"));
	if(EnemyState != EEnemyState::Engaged)
	{
		SetEnemyState(EEnemyState::Chasing);
		GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
		MoveToTarget(CombatTarget);
	}
}

bool AGPEnemy::ShouldChase()
{
	return !IsTargetInRadius(CombatTarget, AttackRadius) && EnemyState != EEnemyState::Chasing;
}

bool AGPEnemy::ShouldAttack()
{
	return IsTargetInRadius(CombatTarget, AttackRadius) && EnemyState != EEnemyState::Attacking && EnemyState !=
		EEnemyState::Engaged;
}

void AGPEnemy::CheckPatrolTarget()
{
	//PatrolTarget 도착
	if(IsTargetInRadius(PatrolTarget, PatrolRadius))
	{
		//새로운 PatrolTarget 설정
		PatrolTarget = ChoosePatrolTarget();
		StartPatrolTimer();
	}
}

void AGPEnemy::CheckCombatTarget()
{
	//CombatTarget이 ComabtRadius 보다 먼 곳에 존재.
	if(!IsTargetInRadius(CombatTarget, CombatRadius))
	{
		ClearAttackTimer();
		//흥미를 잃고 Patrolling 상태로 돌아감
		NoInterest();
	}
	//CombatTarget이 CombatRaiuds 안 쪽에 있으면서 AttackRadius 보다는 먼 상태, 즉 쫓아가야하는 상태
	else if(ShouldChase())
	{
		ClearAttackTimer();
		ChaseCombatTarget();
	}
	//CombatTarget이 AttackRadius 안쪽에 있고 공격 가능한 상태
	else if(ShouldAttack())
	{
		StartAttackTimer();
	}
}

void AGPEnemy::StartAttackTimer()
{
	SetEnemyState(EEnemyState::Attacking);
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AGPEnemy::Attacking, AttackTime);
	UE_LOG(LogTemp, Warning, TEXT("Attacking"));
}

void AGPEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

void AGPEnemy::StartPatrolTimer()
{
	//랜덤 시간 만큼 Wait
	const float WaitTime = FMath::RandRange(PatrolWaitMin, PatrolWaitMax);
	GetWorldTimerManager().SetTimer(PatrolTimer, this, &AGPEnemy::PatrolTimerFinished, WaitTime);
}

void AGPEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

void AGPEnemy::SpawnWeaponClassAndEquip()
{
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		//WeaponClass Spawn
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		//Mesh 소켓에 Attach
		DefaultWeapon->Equip(GetMesh(), FName("RightHandSocket"), this,this);
		//EquippedWeapon으로 설정
		EquippedWeapon = DefaultWeapon;
	}
}

void AGPEnemy::SetHealthBarVisibility(bool Visibility)
{
	if(HealthBarComponent)
	{
		HealthBarComponent->SetVisibility(Visibility);
	}
}



















