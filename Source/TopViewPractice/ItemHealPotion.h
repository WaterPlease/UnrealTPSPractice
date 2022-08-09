// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemHealPotion.generated.h"

/**
 * 
 */
UCLASS()
class TOPVIEWPRACTICE_API AItemHealPotion : public AItem
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	class USoundCue* OverlapSoundCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	float HealAmount;

	AItemHealPotion();

	virtual void OnItemSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
