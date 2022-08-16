// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LeaderboardWidget.generated.h"

/**
 * 
 */
UCLASS()
class TOPVIEWPRACTICE_API ULeaderboardWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* LeaderboardBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* LeaderboardText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Row0;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Row0Score;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Row1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Row1Score;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Row2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Row2Score;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Row3;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Row3Score;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Row4;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Row4Score;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Row5;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Row5Score;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Row6;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Row6Score;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Row7;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Row7Score;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Row8;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Row8Score;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Row9;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Row9Score;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ClearBtn;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void LoadLeaderBoard();

	UFUNCTION(BlueprintCallable)
	void ClearLeaderBoard();
};
