// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseBullet.h"
#include "MinionBullet.generated.h"

/**
 * 
 */
UCLASS()
class TOPVIEWPRACTICE_API AMinionBullet : public ABaseBullet
{
	GENERATED_BODY()
public:
	AMinionBullet();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet | Effect")
	class UParticleSystemComponent* BulletParticleSystemComponent;
};
