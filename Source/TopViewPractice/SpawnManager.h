// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnManager.generated.h"

UCLASS()
class TOPVIEWPRACTICE_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnManager();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn | Config")
	int MaxNumOfEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn | Config")
	TMap<TSubclassOf<class AEnemyCharacter>, float> SpawnList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn | Volume")
	TArray<class ASpawnVolume*> SpawnVolumes;
	
	void IncreaseNumOfEnemy();
	void DecreaseNumOfEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	TArray<float> CDF;
	void BuildCDF();
	int SampleFromCDF();

	TArray<TSubclassOf<class AEnemyCharacter>> SpawnKeyList;

	int NumOfEnemy;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
