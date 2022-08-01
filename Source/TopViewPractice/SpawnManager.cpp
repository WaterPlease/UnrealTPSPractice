// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnManager.h"
#include "SpawnVolume.h"
#include "EnemyCharacter.h"
#include "Algo/RandomShuffle.h"

// Sets default values
ASpawnManager::ASpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.TickInterval = 3.f;
}

void ASpawnManager::IncreaseNumOfEnemy()
{ 
	NumOfEnemy += 1;
}
void ASpawnManager::DecreaseNumOfEnemy() 
{ 
	NumOfEnemy = FMath::Max(0, NumOfEnemy - 1);
}

// Called when the game starts or when spawned
void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();
	
	for (auto SpawnVolume : SpawnVolumes)
	{
		SpawnVolume->SpawnManager = this;
	}

	BuildCDF();
	NumOfEnemy = 0;
}

void ASpawnManager::BuildCDF()
{
	float CurrentProb = 0.f;
	SpawnKeyList.Empty();
	for (auto SpawnPair : SpawnList)
	{
		CurrentProb += SpawnPair.Value;
		SpawnKeyList.Add(SpawnPair.Key);
		CDF.Add(CurrentProb);
	}
}

int ASpawnManager::SampleFromCDF()
{
	float U = FMath::FRandRange(0.f, CDF.Last());
	
	uint32 Min = 0;
	uint32 Max = CDF.Num()-1;
	uint32 Cursor;
	while (Min < Max)
	{
		Cursor = (Min + Max) / 2;
		if (U < CDF[Cursor])
		{
			Max = Cursor;
		}
		else
		{
			Min = Cursor+1;
		}
	}
	Cursor = Max;

	UE_LOG(LogTemp, Warning, TEXT("U : %f     Min-Max-Cursor : %d-%d-%d"), U, Min, Max, Cursor);

	return Cursor;
}

// Called every frame
void ASpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("Enemy count check"));
	Algo::RandomShuffle(SpawnVolumes);

	int N = SpawnList.Num();
	for (int i = 0; i < N && NumOfEnemy<MaxNumOfEnemy; i++)
	{
		TSubclassOf<AEnemyCharacter> EnemyClass = SpawnKeyList[SampleFromCDF()];
		if (SpawnVolumes[i]->SpawnEnemy(EnemyClass))
		{
			IncreaseNumOfEnemy();
		}
	}
}

