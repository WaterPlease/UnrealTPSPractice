// Fill out your copyright notice in the Description page of Project Settings.


#include "Leaderboard.h"
#include "Kismet/GameplayStatics.h"


ULeaderboard::ULeaderboard()
{

}

void ULeaderboard::WriteRecord(TArray<ULeaderboard*>& Rows, FString _PlayerName, int _Score)
{
	ULeaderboard* Row = Cast<ULeaderboard>(UGameplayStatics::CreateSaveGameObject(ULeaderboard::StaticClass()));
	if (!Row) return;
	
	Row->PlayerName = _PlayerName;
	Row->Score = _Score;

	Rows.Add(Row);

	Rows.StableSort(ULeaderboard::SortPredicate);

	UE_LOG(LogTemp, Warning, TEXT("Leaderboard : %s --- %d"), *(Row->PlayerName), Row->Score);

	if (Rows.Num() > 10)
	{
		Rows.RemoveAt(10);
	}

	for (int i = 0; i < Rows.Num(); i++)
	{
		FString SlotName = FString::Printf(TEXT("Leaderboard_%d"), i);
		UGameplayStatics::SaveGameToSlot(Rows[i], SlotName, 0);
		UE_LOG(LogTemp, Warning, TEXT("Leaderboard : Save \"%s\" with Name=\"%s\" and Score=%d"), *SlotName, *(Rows[i]->PlayerName), Rows[i]->Score);
	}
}

void ULeaderboard::LoadRecords(TArray<ULeaderboard*>& Rows)
{
	Rows.Empty();
	for (int i = 0; i < 10; i++)
	{
		FStringFormatNamedArguments args;
		args.Add(TEXT("0"), i);

		FString SlotName = FString::Printf(TEXT("Leaderboard_%d"), i);
		if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
		{
			ULeaderboard* Row = Cast<ULeaderboard>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
			if (Row)
			{
				Rows.Add(Row);
				continue;
			}
		}
		break;
	}

	int i = 0;
	for (auto Row : Rows)
	{
		i += 1;
		//FString TTEST = "TTEST";
		UE_LOG(LogTemp, Warning, TEXT("Leaderboard : %d. %s --- %d"), i, *(Row->PlayerName), Row->Score);
	}
}

void ULeaderboard::ClearRecords()
{
	for (int i = 0; i < 10; i++)
	{
		FStringFormatNamedArguments args;
		args.Add(TEXT("0"), i);

		FString SlotName = FString::Printf(TEXT("Leaderboard_%d"), i);
		if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
		{
			UGameplayStatics::DeleteGameInSlot(SlotName, 0);
			continue;
		}
		break;
	}
}

bool ULeaderboard::SortPredicate(ULeaderboard& A, ULeaderboard& B)
{
	return A.Score > B.Score;
}
