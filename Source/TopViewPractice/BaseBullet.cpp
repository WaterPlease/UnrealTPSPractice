// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ABaseBullet::ABaseBullet()
{

	BulletHead = CreateDefaultSubobject<USphereComponent>(TEXT("BulletHead"));
	SetRootComponent(BulletHead);
	BulletHead->SetCollisionProfileName(TEXT("Projectile"));

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProejectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(BulletHead);
	ProjectileMovementComponent->InitialSpeed = 100.f;
	ProjectileMovementComponent->MaxSpeed = 0.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

	ProjectileMovementComponent->Bounciness = 1.f;
	ProjectileMovementComponent->bShouldBounce = true;

	RemainBounce = 1;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseBullet::BeginPlay()
{
	Super::BeginPlay();

	BulletHead->SetEnableGravity(false);
	BulletHead->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
	BulletHead->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BulletHead->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	BulletHead->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	BulletHead->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	BulletHead->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	BulletHead->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);

	BulletHead->OnComponentHit.AddDynamic(this, &ABaseBullet::OnBulletHeadHit);
	BulletHead->OnComponentBeginOverlap.AddDynamic(this, &ABaseBullet::OnBulletHeadOverlapBegin);
	BulletHead->OnComponentEndOverlap.AddDynamic(this, &ABaseBullet::OnBulletHeadOverlapEnd);
}

void ABaseBullet::LauchBullet(const FVector& Direction)
{
	ProjectileMovementComponent->InitialSpeed = InitialSpeed;
	ProjectileMovementComponent->Velocity = InitialSpeed * Direction.GetSafeNormal();
	UE_LOG(LogTemp, Warning, TEXT("Bullet depart : %f -> (%f,%f,%f)"),
		ProjectileMovementComponent->Velocity.Size(),
		ProjectileMovementComponent->Velocity.X,
		ProjectileMovementComponent->Velocity.Y,
		ProjectileMovementComponent->Velocity.Z);

	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ABaseBullet::DestroyBullet, LifeTime);
}

void ABaseBullet::DestroyBullet()
{
	GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
	Destroy();
}

// Called every frame
void ABaseBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseBullet::OnBulletHeadOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == Shooter) return;
	DestroyBullet();
}

void ABaseBullet::OnBulletHeadOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//
}

void ABaseBullet::OnBulletHeadHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Spawn decal
	// ...

	// Bounce
	if (!RemainBounce)
	{
		DestroyBullet();
	}
	RemainBounce -= 1;
}

