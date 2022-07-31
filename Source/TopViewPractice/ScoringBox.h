// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoringBox.generated.h"

/**
 * 
 */
UCLASS()
class TOPVIEWPRACTICE_API UScoringBox : public UUserWidget
{
	GENERATED_BODY()

public:

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* AddedScoreText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* ScoreTextContainer;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* AddScoreAnimBegin;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* AddScoreAnimEnd;

public:
	void PlayScoreingBeginAnimation();
	void PlayScoreingEndAnimation();

	void SetTextOpacity(float Opacity);
	
	void SetAddedScore(int Score);
};
