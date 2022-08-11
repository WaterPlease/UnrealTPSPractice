// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuController.generated.h"

/**
 * 
 */
UCLASS()
class TOPVIEWPRACTICE_API AMainMenuController : public APlayerController
{
	GENERATED_BODY()
public:
	AMainMenuController();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> MainMenuAsset;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UUserWidget* MainMenu;

protected:
	virtual void BeginPlay() override;
};
