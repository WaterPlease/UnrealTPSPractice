// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterController.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameUserSettings.h"
#include "InGameHUD.h"
#include "Camera/CameraComponent.h"
#include "EnemyCharacter.h"

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

	InGameHUD = Cast<AInGameHUD>(GetHUD());
}

void APlayerCharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update In-Game HUD
	if (InGameHUD &&
		GetPlayer())
	{
		InGameHUD->CrosshairLineOffset = Player->CrosshairOffset;
		InGameHUD->bHitmarkerDisplay = Player->bHitmarkerDisplay;

		FMinimalViewInfo ViewInfo;
		Player->GetCameraComponent()->GetCameraView(GetWorld()->DeltaTimeSeconds, ViewInfo);
		FMatrix ProjectionMatrix = ViewInfo.CalculateProjectionMatrix();
		FVector4 ProjectedPosition = ProjectionMatrix.TransformPosition(FVector(Player->Spread, 0.f, 100.f));
		ProjectedPosition = ProjectedPosition / ProjectedPosition.W;
		FVector2D Result;
		GEngine->GameViewport->GetViewportSize(Result);
		InGameHUD->CrosshairLineOffset = ProjectedPosition.X * Result.X * 0.5f;
		InGameHUD->CursorOffset = Player->CursorOffset;
	}
}

APlayerCharacter* APlayerCharacterController::GetPlayer()
{
	if (!Player)
	{
		Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}

	return Player;
}

void APlayerCharacterController::AddScore(int ScoreAmount)
{
	Score += ScoreAmount;
}

void APlayerCharacterController::EnemyKill(const AEnemyCharacter* const Enemy)
{
	if (!Enemy) return;

	AddScore(Enemy->Score);
}


