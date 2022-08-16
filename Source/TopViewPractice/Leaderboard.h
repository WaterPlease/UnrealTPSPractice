// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Leaderboard.generated.h"

/**
 * 
 */
UCLASS()
class TOPVIEWPRACTICE_API ULeaderboard : public USaveGame
{
	GENERATED_BODY()
public:
	ULeaderboard();

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	int Score;

	UFUNCTION(BlueprintCallable)
	static void WriteRecord(TArray<ULeaderboard*>& Rows, FString _PlayerName, int _Score);
	UFUNCTION(BlueprintCallable)
	static void LoadRecords(TArray<ULeaderboard*>& rows);
	UFUNCTION(BlueprintCallable)
	static void ClearRecords();

	static bool SortPredicate(ULeaderboard& A, ULeaderboard& B);
};