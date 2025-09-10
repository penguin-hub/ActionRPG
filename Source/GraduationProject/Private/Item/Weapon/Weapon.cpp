// C135203_GraduationProject

#include "Item/Weapon/Weapon.h"
#include "NiagaraComponent.h"
#include "Character/GPCharacter.h"
#include "Component/InventorySystemComponent.h"
#include "Component/ItemDataComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interface/HitInterface.h"
#include "Item/ItemStruct.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
	//Set Box
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(GetRootComponent());
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	//Set Trace Points
	TraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Trace Start"));
	TraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Trace End"));
	TraceStart->SetupAttachment(GetRootComponent());
	TraceEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	//ItemDataTable 의 해당 무기의 Damage 만큼 Damage Set
	SetWeaponDamage();
	
	//Box의 ComponentBeginOverlap 함수 바인딩
	Box->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxBeginOverlap);
}

void AWeapon::AttachToSocket(USceneComponent* InParent, FName InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	PlayEquipSound(NewOwner);

	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	
	AttachToSocket(InParent, InSocketName);
	
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if(ItemEffect)
	{
		ItemEffect->Deactivate();
	}
}

void AWeapon::ItemInteract(AActor* InteractActor)
{
	AGPCharacter* GPCharacter = Cast<AGPCharacter>(InteractActor);
	if(GPCharacter)
	{
		//인벤토리에 추가
		GPCharacter->Inventory->AddToInventory(ItemDataComponent->ItemData);
		GPCharacter->SetCharacterState(ECharacterState::EquippedWeapon);
		USkeletalMeshComponent* GPCharacterMesh = GPCharacter->GetMesh();
		if(GPCharacter->EquippedWeapon)
			GPCharacter->EquippedWeapon->Destroy();
		this->Equip(GPCharacterMesh, FName("RightHandSocket"), GPCharacter, GPCharacter);
		GPCharacter->EquippedWeapon = this;
		GPCharacter->OverlappingItem = nullptr;
	}
}

void AWeapon::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(GetOwner()->ActorHasTag(FName("GPEnemy")) && OtherActor->ActorHasTag(FName("GPEnemy")))
		return;
	if(GetOwner()->ActorHasTag(FName("GPCharacter")) && OtherActor->ActorHasTag(FName("GPCharacter")))
		return;

	//언리얼의 BoxTraceSingle함수에서 참조로 인풋으로 받아서 안의 값을 바꾸어 리턴 해준다.
	FHitResult BoxHitResult;
	DoBoxTrace(BoxHitResult);
	
	if(BoxHitResult.GetActor())
	{
		if(GetOwner()->ActorHasTag(FName("GPEnemy")) && BoxHitResult.GetActor()->ActorHasTag(FName("GPEnemy")))
			return;
		if(GetOwner()->ActorHasTag(FName("GPCharacter")) && OtherActor->ActorHasTag(FName("GPCharacter")))
			return;
		if(OtherActor->ActorHasTag(FName("Die")))
			return;
		//오버랩 된 액터가 HitInterface 포함 하는지 확인 후에 인터페이스 함수 실행
		IHitInterface* HitInterface = Cast<IHitInterface>(BoxHitResult.GetActor());
		if(HitInterface)
		{
			//Apply Damage, TakeDamage 발동
			UGameplayStatics::ApplyDamage(BoxHitResult.GetActor(), Damage, GetInstigator()->GetController(),
				this, UDamageType::StaticClass());
			//HitInterface GetHit 함수 실행
			HitInterface->Execute_GetHit(BoxHitResult.GetActor(), BoxHitResult.ImpactPoint);
		}
		//Hit된 액터 IgnoreActors 에 추가, 중복 Hit 방지 위해서, IgnoreActors는 공격 애니메이션이 끝날 때 마다 초기화 됨
		IgnoreActors.AddUnique(BoxHitResult.GetActor());

		//Hit 했을 때 필드 만들어줌, Breakable Actor Break 위해서
		CreateField(BoxHitResult.ImpactPoint);
	}
}

void AWeapon::DoBoxTrace(FHitResult& BoxHitResult)
{
	const FVector Start = TraceStart->GetComponentLocation();
	const FVector End = TraceEnd->GetComponentLocation();
	//TArray 동적으로 크기 변하는 배열, unreal engine built-in array type
	//Trace 시에 전달 할 인자 중 Ignore 할 Actor들을 전달하기 위해 생성
	TArray<AActor*> ActorsToIgnore;
	//자기 자신과 Hit 하지 않도록 추가
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());
	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}
	//Trace 수행
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(20.f, 20.f, 20.f),
		TraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		BoxHitResult,
		true
	);
}

void AWeapon::SetWeaponDamage()
{
	const UDataTable* ItemDataTable = ItemDataComponent->ItemData.ItemDataTableRowHandle.DataTable;
	if(ItemDataTable)
	{
		FItemStruct* ItemStruct = ItemDataTable->FindRow<FItemStruct>(ItemDataComponent->ItemData.ItemDataTableRowHandle.RowName, "");
		if(ItemStruct)
		{
			Damage = ItemStruct->Damage;
		}
	}
}

void AWeapon::PlayEquipSound(AActor* NewOwner)
{
	if(InteractSound)
	{
		if(!NewOwner->ActorHasTag(FName("GPEnemy")))
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), InteractSound, this->GetActorLocation());
	}
}
