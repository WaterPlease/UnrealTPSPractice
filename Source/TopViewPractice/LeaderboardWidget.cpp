// Fill out your copyright notice in the Description page of Project Settings.


#include "LeaderboardWidget.h"
#include "GameFramework/Actor.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Leaderboard.h"

void ULeaderboardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<ULeaderboard*> Rows;
	ULeaderboard::LoadRecords(Rows);

	UTextBlock* RowWidgets[10] = { Row0, Row1, Row2, Row3, Row4, Row5, Row6, Row7, Row8, Row9};
	UTextBlock* RowScoreWidgets[10] = { Row0Score, Row1Score, Row2Score, Row3Score, Row4Score, Row5Score, Row6Score, Row7Score, Row8Score, Row9Score };

	for (int i = 0; i < Rows.Num(); i++)
	{
		FString RowText = FString::Printf(TEXT("%2d. %-10s"), i + 1, *(Rows[i]->PlayerName));
		RowWidgets[i]->SetText(FText::FromString(RowText));

		RowText = FString::Printf(TEXT("%10d"), Rows[i]->Score);
		RowScoreWidgets[i]->SetText(FText::FromString(RowText));
	}
}
