// Fill out your copyright notice in the Description page of Project Settings.
#include "SpawnManager.h"
#include "SpawnVolume.h"
#include "EnemyCharacter.h"
#include "Algo/RandomShuffle.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacterController.h"
#include "Item.h"

// Sets default values
ASpawnManager::ASpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.TickInterval = 3.f;

	Round = 0;
	bSpawn = false;
}

void ASpawnManager::NextRound()
{
	Round += 1;
	MaxNumOfEnemy += 1;

	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(),0));

	if (PlayerController)
	{
		PlayerController->NextRound(Round, BreakTime);
	}
	SpawnedEnemy = 0;
	KilledEnemy = 0;
	bSpawn = false;
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnManager::StartSpawn, BreakTime);
}

void ASpawnManager::AddKilledEnemy()
{
	KilledEnemy++;

	if (KilledEnemy == MaxNumOfEnemy)
	{
		TArray<AActor*> EnemyCharacters;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), EnemyCharacters);
		for (auto _Enemy : EnemyCharacters)
		{
			auto Enemy = Cast<AEnemyCharacter>(_Enemy);

			if (Enemy)
			{
				Enemy->Score = 0.f;
				Enemy->Die();
			}
		}
		NextRound();
	}
}

TSubclassOf<AItem> ASpawnManager::SampleDropItem()
{
	TSubclassOf<AItem> SampledItem = DropKeyList[SampleFromCDF(DropCDF)];
	return SampledItem;
}

void ASpawnManager::StartSpawn()
{
	bSpawn = true;
}

// Called when the game starts or when spawned
void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();
	
	for (auto SpawnVolume : SpawnVolumes)
	{
		SpawnVolume->SpawnManager = this;
	}

	BuildCDF(SpawnList,SpawnCDF, SpawnKeyList);
	BuildCDF(DropList, DropCDF, DropKeyList);
	NextRound();
}

template<typename TCLASS>
void ASpawnManager::BuildCDF(const TMap<TSubclassOf<TCLASS>, float>& SelectList, TArray<float>& CDF, TArray<TSubclassOf<TCLASS>>& SelectKeyList)
{
	float CurrentProb = 0.f;
	SelectKeyList.Empty();
	for (auto SelectPair : SelectList)
	{
		CurrentProb += SelectPair.Value;
		SelectKeyList.Add(SelectPair.Key);
		CDF.Add(CurrentProb);
	}
}

int ASpawnManager::SampleFromCDF(const TArray<float>& CDF)
{
	float U = FMath::FRandRange(0.f, CDF.Last());
	
	uint32 Min = 0;
	uint32 Max = CDF.Num()-1;
	uint32 Cursor;

	// binary search
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

	if (bSpawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy count check"));
		Algo::RandomShuffle(SpawnVolumes);

		int N = SpawnVolumes.Num();
		for (int i = 0; i < N && SpawnedEnemy < MaxNumOfEnemy; i++)
		{
			TSubclassOf<AEnemyCharacter> EnemyClass = SpawnKeyList[SampleFromCDF(SpawnCDF)];
			if (SpawnVolumes[i]->SpawnEnemy(EnemyClass))
			{
				SpawnedEnemy += 1;
			}
		}
	}
}

