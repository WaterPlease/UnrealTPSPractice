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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn | Config")
	int SpawnedEnemy;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn | Config")
	int KilledEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn | Config")
	float BreakTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn | Config")
	TMap<TSubclassOf<class AEnemyCharacter>, float> SpawnList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn | Config")
	TMap<TSubclassOf<class AItem>, float> DropList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn | Volume")
	TArray<class ASpawnVolume*> SpawnVolumes;
	
	void NextRound();

	void AddKilledEnemy();

	TSubclassOf<AItem> SampleDropItem();

	void StartSpawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn | Volume")
		TArray<TSubclassOf<AEnemyCharacter>> SpawnKeyList;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn | Volume")
		TArray<float> SpawnCDF;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	TArray<float> DropCDF;
	
	template<typename TCLASS>
	void BuildCDF(const TMap<TSubclassOf<TCLASS>, float>& SelectList, TArray<float>& CDF, TArray<TSubclassOf<TCLASS>>& SelectKeyList);

	int SampleFromCDF(const TArray<float>& CDF);

	bool bSpawn;
	TArray<TSubclassOf<AItem>> DropKeyList;

	int Round;

	FTimerHandle SpawnTimer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};


