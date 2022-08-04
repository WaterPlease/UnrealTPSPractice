// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemStunGrenade.h"
#include "PlayerCharacter.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"


AItemStunGrenade::AItemStunGrenade()
{

}

void AItemStunGrenade::OnItemSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);

	if (Player)
	{
		Player->StunGrenadeRemain += 1;

		if (OverlapSoundCue)
			UGameplayStatics::PlaySound2D(GetWorld(), OverlapSoundCue);

		Destroy();
	}
}