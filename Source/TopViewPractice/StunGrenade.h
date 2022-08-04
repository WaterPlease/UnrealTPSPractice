// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGrenade.h"
#include "StunGrenade.generated.h"

/**
 * 
 */
UCLASS()
class TOPVIEWPRACTICE_API AStunGrenade : public ABaseGrenade
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stun")
	float StunTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stun")
	float StunFactor;
private:
	TArray<class AEnemyCharacter*> StunnedEnemies;
protected:
	virtual void BeginPlay() override;
public:
	AStunGrenade();

	virtual void Detonate() override;

	FTimerHandle StunTimerHandle;

	void RestoreSpeed();
};
