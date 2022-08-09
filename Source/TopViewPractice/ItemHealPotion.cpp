// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemHealPotion.h"
#include "PlayerCharacter.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

AItemHealPotion::AItemHealPotion()
{
	HealAmount = 10.f;
}

void AItemHealPotion::OnItemSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);

	if (Player)
	{
		Player->Health += HealAmount;
		Player->bHeal = true;

		if (OverlapSoundCue)
			UGameplayStatics::PlaySound2D(GetWorld(), OverlapSoundCue);

		Destroy();
	}
}