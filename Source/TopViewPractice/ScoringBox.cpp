// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoringBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacterController.h"

void UScoringBox::NativeConstruct()
{
	Super::NativeConstruct();

	ScoreTextContainer->AddChildToVerticalBox(AddedScoreText);
	SetTextOpacity(0.f);
	APlayerCharacterController* PlayerController = 
		Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController)
	{
		PlayerController->WidgetScoreBox = this;
	}
}

void UScoringBox::PlayScoreingBeginAnimation()
{
	PlayAnimation(AddScoreAnimBegin);
}

void UScoringBox::PlayScoreingEndAnimation()
{
	PlayAnimation(AddScoreAnimEnd);
}

void UScoringBox::SetTextOpacity(float Opacity)
{
	AddedScoreText->SetOpacity(Opacity);
}

void UScoringBox::SetAddedScore(int Score)
{
	#define LOCTEXT_NAMESPACE "MyNamespace"
	FText Text = FText::Format(LOCTEXT("AddedScoreText", "+ {0}"), Score);
	#undef LOCTEXT_NAMESPACE
	AddedScoreText->SetText(Text);
	PlayAnimation(AddScoreAnimBegin);
}
