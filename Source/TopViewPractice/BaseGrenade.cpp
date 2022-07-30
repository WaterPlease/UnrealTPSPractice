// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGrenade.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Sound/SoundCue.h"

// Sets default values
ABaseGrenade::ABaseGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
	Sphere->SetMobility(EComponentMobility::Movable);
	Sphere->SetSimulatePhysics(true);
	Sphere->SetEnableGravity(true);

	EffectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("EffectSphere"));
	EffectSphere->SetupAttachment(Sphere);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Sphere);

	DetonationDelay = 2.0f;
	Damage = 110.f;
	ImpulseCharacterStrength = 2000.f;
	ImpulsePropStrength = 2000.f;
}

// Called when the game starts or when spawned
void ABaseGrenade::BeginPlay()
{
	Super::BeginPlay();

	Sphere->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Sphere->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
	//Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic,ECollisionResponse::ECR_Block);
	//Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);

	EffectSphere->SetCollisionProfileName(FName("OverlapAllDynamic"));
	EffectSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);

	GetWorldTimerManager().SetTimer(DetonationTimerHandle, this, &ABaseGrenade::Detonate, DetonationDelay);	
}

// Called every frame
void ABaseGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugSphere(GetWorld(),GetActorLocation(), EffectSphere->GetScaledSphereRadius(),10,FColor::Red,false,-1.f,0U,3.f);
}

void ABaseGrenade::Detonate()
{
	TArray<FHitResult> HitResults;
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		GetActorLocation(), GetActorLocation(),FQuat::Identity,
		ECC_WorldStatic,
		EffectSphere->GetCollisionShape()
	);

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (bHit)
	{
		for (auto HitResult : HitResults)
		{
			AActor* HitActor = HitResult.GetActor();

			if (!HitActor)
			{
				// Ignore something not having root actor like Brush.
				continue;
			}

			ACharacter* Character = Cast<ACharacter>(HitActor);
			if (Character)
			{
				UGameplayStatics::ApplyDamage(Character, Damage, PlayerController, this, DamageType);
				Character->GetMesh()->AddRadialImpulse(GetActorLocation(), EffectSphere->GetScaledSphereRadius(), ImpulseCharacterStrength, ERadialImpulseFalloff::RIF_Constant, true);
				continue;
			}

			UActorComponent* HitActorComponent = HitActor->GetComponentByClass(UStaticMeshComponent::StaticClass());
			UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(HitActorComponent);
			if (MeshComponent)
			{
				UGameplayStatics::ApplyDamage(HitActor, Damage, PlayerController, this, DamageType);
				MeshComponent->AddRadialImpulse(GetActorLocation(), EffectSphere->GetScaledSphereRadius(), ImpulsePropStrength, ERadialImpulseFalloff::RIF_Constant,true);
				continue;
			}

			// Other Cases
			// ....
		}
	}

	if (DetonationParticle)
	{
		FRotator ParticleRotator = FRotator::ZeroRotator;
		ParticleRotator.Yaw = FMath::FRandRange(-180.f, 180.f);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DetonationParticle, GetActorLocation(), ParticleRotator, FVector(1.f));
	}

	if (ExplosionSoundCue)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSoundCue, GetActorLocation());
	}

	Destroy();
}

