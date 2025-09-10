// C135203_GraduationProject

#include "Item/Item.h"
#include "Character/GPCharacter.h"
#include "Component/InventorySystemComponent.h"
#include "Component/ItemDataComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "HUD/ShowTextComponent.h"
#include "Item/ItemStruct.h"
#include "Kismet/GameplayStatics.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;
	//Set ItemMesh
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = ItemMesh;
	//Set Sphere
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());
	//Set ItemDataComponent
	ItemDataComponent = CreateDefaultSubobject<UItemDataComponent>(TEXT("ItemDataComponent"));
	//Set ItemEffect
	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ItemEffect"));
	ItemEffect->SetupAttachment(GetRootComponent());
	//Set ShowTextComponent
	ShowTextComponent = CreateDefaultSubobject<UShowTextComponent>(TEXT("ShowTextComponent"));
	ShowTextComponent->SetupAttachment(GetRootComponent());
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	if(ShowTextComponent)
	{
		ShowTextComponent->SetVisibility(false);
	}
	//OnComponentBeginOverlap delegate에 callback 을 bind
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}

void AItem::ItemInteract(AActor* InteractActor)
{
	if(InteractSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), InteractSound, this->GetActorLocation());
	}
	//AddToInventory 후 Item Destroy
	AGPCharacter* InteractCharacter = Cast<AGPCharacter>(InteractActor);
	if(InteractCharacter)
	{
		InteractCharacter->Inventory->AddToInventory(ItemDataComponent->ItemData);
		this->Destroy();
	}
}

void AItem::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                 AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	if(ShowTextComponent)
		ShowTextComponent->SetVisibility(true);
	AGPCharacter* GPCharacter = Cast<AGPCharacter>(OtherActor);
	if(GPCharacter && GPCharacter->OverlappingItem == this)
		return;
	SetOverlappingItem(OtherActor);
	//아이템 이름 보여주기
	//DebugItemNameOnScreen();
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(ShowTextComponent)
		ShowTextComponent->SetVisibility(false);
	SetOverlappingItemNull(OtherActor);
}

void AItem::DebugItemNameOnScreen()
{
	if(ItemDataComponent->ItemData.ItemDataTableRowHandle.DataTable)
	{
		FItemStruct* ItemStruct = ItemDataComponent->ItemData.ItemDataTableRowHandle.DataTable->FindRow<FItemStruct>(ItemDataComponent->ItemData.ItemDataTableRowHandle.RowName, "");
		if(ItemStruct)
		{
			FText ItemName = ItemStruct->Name;
			if(GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, ItemName.ToString());
		}
	}
}

void AItem::SetOverlappingItem(AActor* OtherActor)
{
	AGPCharacter* GPCharacter = Cast<AGPCharacter>(OtherActor);
	if(GPCharacter)
	{
		GPCharacter->SetOverlappingItem(this);
	}
}

void AItem::SetOverlappingItemNull(AActor* OtherActor)
{
	AGPCharacter* GPCharacter = Cast<AGPCharacter>(OtherActor);
	if(GPCharacter)
	{
		GPCharacter->SetOverlappingItem(nullptr);
	}
}


