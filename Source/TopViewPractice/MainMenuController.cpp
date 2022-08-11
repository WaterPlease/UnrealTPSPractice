// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuController.h"
#include "Blueprint/UserWidget.h"

AMainMenuController::AMainMenuController()
{

}

void AMainMenuController::BeginPlay()
{
	if (MainMenuAsset)
	{
		MainMenu = CreateWidget<UUserWidget>(this, MainMenuAsset);
		if (MainMenu)
		{
			MainMenu->AddToViewport();
			MainMenu->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
