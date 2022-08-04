// Fill out your copyright notice in the Description page of Project Settings.


#include "StunGrenade.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Sound/SoundCue.h"
#include "EnemyCharacter.h"
#include "UObject/WeakObjectPtrTemplates.h"

void AStunGrenade::BeginPlay()
{
	Super::BeginPlay();
}

AStunGrenade::AStunGrenade()
{
	StunTime = 5.f;
	StunFactor = 0.1f;
}

void AStunGrenade::Detonate()
{
	TArray<FHitResult> HitResults;
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		GetActorLocation(), GetActorLocation(), FQuat::Identity,
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

			AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(HitActor);
			if (Enemy && !StunnedEnemies.Contains(Enemy))
			{
				UE_LOG(LogTemp,Warning,TEXT("Stun : %f * %f"), Enemy->GetCharacterMovement()->MaxWalkSpeed, StunFactor)
				Enemy->GetCharacterMovement()->MaxWalkSpeed = Enemy->GetCharacterMovement()->MaxWalkSpeed * StunFactor;
				UE_LOG(LogTemp, Warning, TEXT("Stun : = %f"), Enemy->GetCharacterMovement()->MaxWalkSpeed * StunFactor)
				StunnedEnemies.Add(Enemy);
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

	GetWorldTimerManager().SetTimer(StunTimerHandle, this, &AStunGrenade::RestoreSpeed, StunTime);
}

void AStunGrenade::RestoreSpeed()
{
	float InvStunFactor = 1.f / StunFactor;
	for (auto Enemy : StunnedEnemies)
	{
		if (Enemy && IsValid(Enemy))
			Enemy->GetCharacterMovement()->MaxWalkSpeed = Enemy->GetCharacterMovement()->MaxWalkSpeed * InvStunFactor;
	}

	Destroy();
}

