// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "EnemyCharacter.h"
#include "EnemyCharacter.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}

bool ASpawnVolume::SpawnEnemy(const TSubclassOf<class AEnemyCharacter>& EnemyClass)
{
	AEnemyCharacter* Enemy = GetWorld()->SpawnActor<AEnemyCharacter>(EnemyClass, GetActorLocation(), FRotator(0.f));
	Enemy->SpawnManager = SpawnManager;
	return true;
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

