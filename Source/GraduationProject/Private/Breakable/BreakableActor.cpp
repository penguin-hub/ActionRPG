// C135203_GraduationProject

#include "Breakable/BreakableActor.h"
#include "Components/CapsuleComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Item/Item.h"
#include "Kismet/GameplayStatics.h"

ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	//Set GeometryCollectionComponent
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(GeometryCollection);
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	//SetCapsule
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint)
{
	//이미 Broken이면 return
	if(bBroken)
		return;
	bBroken = true;

	PlayBreakSound(ImpactPoint);
	SpawnItemClass();
}

void ABreakableActor::SpawnItemClass()
{
	UWorld* World = GetWorld();
	if(World && ItemClassArray.Num() > 0)
	{
		FVector Location = GetActorLocation();
		Location.Z += 75.f;

		const int32 randint = FMath::RandRange(0, ItemClassArray.Num() - 1);
		World->SpawnActor<AItem>(ItemClassArray[randint], Location, GetActorRotation());
	}
}

void ABreakableActor::PlayBreakSound(const FVector& ImpactPoint)
{
	if(BreakSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), BreakSound, ImpactPoint);
	}
}