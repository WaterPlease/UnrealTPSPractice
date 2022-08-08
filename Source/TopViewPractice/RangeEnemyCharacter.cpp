// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeEnemyCharacter.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BaseBullet.h"

ARangeEnemyCharacter::ARangeEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bTryLookAt = true;
}

void ARangeEnemyCharacter::Fire()
{
	FVector SocketLocation = GetMesh()->GetSocketLocation(FName("Muzzle_Front"));

	FVector BulletDirection = FindBulletDirection(SocketLocation);

	ABaseBullet* Bullet = GetWorld()->SpawnActor<ABaseBullet>(BulletType, SocketLocation, BulletDirection.Rotation());
	Bullet->Shooter = (AActor*)this;
	Bullet->Instigator = AIController;
	Bullet->bShotByPlayer = false;
	Bullet->Damage = Damage;
	Bullet->InitialSpeed = BulletSpeed;
	Bullet->LauchBullet(BulletDirection);
}

FVector ARangeEnemyCharacter::FindBulletDirection(const FVector& E)
{
	if (!GetPlayerCharacter()) return FVector(0.f);

	FVector P = GetPlayerCharacter()->GetActorLocation();
	FVector V = GetPlayerCharacter()->GetVelocity();
	FVector N = (P - E).GetSafeNormal();
	float NdotV = FVector::DotProduct(N, V);
	float VdotV = FVector::DotProduct(V, V);
	float k = -NdotV + FMath::Sqrt(NdotV*NdotV + BulletSpeed*BulletSpeed - VdotV);

	return (N * k + V) / BulletSpeed;
}

void ARangeEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	LookAtSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ARangeEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARangeEnemyCharacter::Attack()
{
	Super::Attack();
}