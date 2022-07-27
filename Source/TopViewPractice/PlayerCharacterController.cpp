// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterController.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

APlayerCharacterController::APlayerCharacterController()
{

}

void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();

	GetPlayer();
	if (InGameScreenWidgetAsset)
	{
		InGameScreenWidget = CreateWidget<UUserWidget>(this, InGameScreenWidgetAsset);
		if (InGameScreenWidget)
		{
			InGameScreenWidget->AddToViewport();
			InGameScreenWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void APlayerCharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

APlayerCharacter* APlayerCharacterController::GetPlayer()
{
	if (!Player)
	{
		Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}

	return Player;
}
