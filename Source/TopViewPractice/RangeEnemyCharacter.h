// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "RangeEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TOPVIEWPRACTICE_API ARangeEnemyCharacter : public AEnemyCharacter
{
	GENERATED_BODY()
	
public:
	ARangeEnemyCharacter();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy | Stats")
	float MaxFireRange;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy | Stats")
	float ClosestDistance;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy | Combat")
	TSubclassOf<class ABaseBullet> BulletType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy | Combat")
	class UParticleSystem* MuzzleFlashParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy | Combat")
	float BulletSpeed;

	UFUNCTION(BlueprintCallable)
	void Fire();

	FVector FindBulletDirection(const FVector& E);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Attack() override;
};
