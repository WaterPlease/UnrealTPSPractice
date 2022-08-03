// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameRoundIndicator.generated.h"

/**
 * 
 */
UCLASS()
class TOPVIEWPRACTICE_API UInGameRoundIndicator : public UUserWidget
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* RoundText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* RoundNumberText;
};
