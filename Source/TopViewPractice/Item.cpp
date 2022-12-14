// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	// Compoenent initialization
	ItemSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ItemSphere"));
	SetRootComponent(ItemSphere);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ItemParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ItemParticleSystem"));
	ItemParticleSystem->SetupAttachment(GetRootComponent());
	
	// Parameter initialization
	bHighlighten = false;

	AutoDestroyDelay = 60.f;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	ItemSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ItemSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	ItemSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Overlap);
	ItemSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	ItemSphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnItemSphereOverlapBegin);
	ItemSphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnItemSphereOverlapEnd);

	ItemParticleSystem->SetTemplate(IdleItemParticleSystem);

	if (AutoDestroyDelay > 0.f)
	{
		GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AItem::AutoDestroy, AutoDestroyDelay);
	}
}

void AItem::AutoDestroy()
{
	Destroy();
}

bool AItem::ToggleHighlight()
{
	bHighlighten = !bHighlighten;

	if (bHighlighten)
	{
		ItemParticleSystem->SetTemplate(HighlightenItemParticleSystem);
	}
	else
	{
		ItemParticleSystem->SetTemplate(IdleItemParticleSystem);
	}

	return bHighlighten;
}

void AItem::OnItemSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ToggleHighlight();
	GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
}

void AItem::OnItemSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ToggleHighlight();
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AItem::AutoDestroy, AutoDestroyDelay);
}

