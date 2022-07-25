// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Particles/ParticleSystem.h"
#include "PlayerCharacter.h"
#include "BaseBullet.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
	
}

void AWeapon::Fire(const FVector& Location, const FVector& Direction)
{
	ABaseBullet* Bullet = GetWorld()->SpawnActor<ABaseBullet>(BulletType, Location, FRotator(0.f));
	Bullet->Shooter = (AActor*)UGameplayStatics::GetPlayerPawn(GetWorld(),0);
	Bullet->LauchBullet(Direction);
}

void AWeapon::OnItemSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnItemSphereOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	APlayerCharacter* Player;
	Player = Cast<APlayerCharacter>(OtherActor);

	if (!Player) return;

	Player->WeaponToEquipped = this;
}

void AWeapon::OnItemSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnItemSphereOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	APlayerCharacter* Player;
	Player = Cast<APlayerCharacter>(OtherActor);

	if (!Player) return;

	Player->WeaponToEquipped = nullptr;


	// Find other overlapped weapon
	TSet<AActor*> OverlappedWeapons;
	Player->GetOverlappingActors(OverlappedWeapons, AWeapon::StaticClass());
	float MinDist = 9999.f;
	AWeapon* ClosestWeapon = nullptr;
	for (auto Weapon : OverlappedWeapons)
	{
		if ((AWeapon*)Weapon == Player->EquippedWeapon) continue;
		float Dist = FVector::Dist2D(Player->GetActorLocation(), Weapon->GetActorLocation());

		if (Dist < MinDist)
		{
			MinDist = Dist;
			ClosestWeapon = Cast<AWeapon>(Weapon);
		}
	}

	if (ClosestWeapon)
	{
		Player->WeaponToEquipped = ClosestWeapon;
	}
}
