// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterController.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameUserSettings.h"
#include "InGameHUD.h"
#include "Camera/CameraComponent.h"
#include "EnemyCharacter.h"
#include "ScoringBox.h"
#include "Weapon.h"
#include "GameFramework/PlayerController.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnemyHealthBar.h"
#include "Components/ProgressBar.h"
#include "Components/CapsuleComponent.h"

APlayerCharacterController::APlayerCharacterController()
{
	AddedScore = 0;
	Score = 0;
	Round = 1;
	RoundTimer = -1.f;
}

void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());

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
	if (PerkSelectWidgetAsset)
	{
		PerkSelectWidget = CreateWidget<UUserWidget>(this, PerkSelectWidgetAsset);
		if (PerkSelectWidget)
		{
			PerkSelectWidget->AddToViewport();
			PerkSelectWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	if (WeaponEquipWidgetAsset)
	{
		WeaponEquipWidget = CreateWidget<UUserWidget>(this, WeaponEquipWidgetAsset);
		if (WeaponEquipWidget)
		{
			WeaponEquipWidget->AddToViewport();
			WeaponEquipWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (EnemyHealthBarAsset)
	{
		EnemyHealthBar = CreateWidget<UEnemyHealthBar>(this, EnemyHealthBarAsset);
		if (EnemyHealthBar)
		{
			EnemyHealthBar->SetDesiredSizeInViewport(EnemyHealthBarSize);
			EnemyHealthBar->AddToViewport();
			EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
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

	if (WeaponEquipWidget &&
		GetPlayer())
	{
		if (Player->WeaponToEquipped)
		{
			AWeapon* Weapon = Player->WeaponToEquipped;
			FVector WeaponLocation = Weapon->GetActorLocation();
			WeaponLocation.Z += Weapon->ItemSphere->GetScaledSphereRadius();
			FVector2D ScreenPos;
			UGameplayStatics::ProjectWorldToScreen(this, WeaponLocation, ScreenPos);
			WeaponEquipWidget->SetPositionInViewport(ScreenPos);
			WeaponEquipWidget->SetVisibility(ESlateVisibility::Visible);
		}
		else if (Player->bCanExit)
		{
			FVector CapsuleLocation = Player->GetCapsuleComponent()->GetComponentLocation();
			CapsuleLocation.Z += Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
			FVector2D ScreenPos;
			UGameplayStatics::ProjectWorldToScreen(this, CapsuleLocation, ScreenPos);
			WeaponEquipWidget->SetPositionInViewport(ScreenPos);
			WeaponEquipWidget->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			WeaponEquipWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (EnemyHealthBar &&
		GetPlayer())
	{
		if (Player->EnemyOnCursor)
		{
			AEnemyCharacter* Enemy = Player->EnemyOnCursor;
			FVector EnemyLocation = Enemy->GetActorLocation();
			EnemyLocation.Z += Enemy->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 1.2f;;
			FVector2D ScreenPos;
			UGameplayStatics::ProjectWorldToScreen(this, EnemyLocation, ScreenPos);
			ScreenPos.X -= EnemyHealthBarSize.X * 0.25f;
			EnemyHealthBar->SetPositionInViewport(ScreenPos);
			EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
			EnemyHealthBar->HealthBar->SetPercent(Enemy->Health / Enemy->MaxHealth);
		}
		else
		{
			EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (RoundTimer > 0.f)
	{
		RoundTimer -= DeltaTime;

		if (RoundTimer < 0.f || FMath::IsNearlyZero(RoundTimer))
		{
			RoundTimer = -1.f;
		}
	}
}

void APlayerCharacterController::ScoringReset()
{
	AddedScore = 0;
	WidgetScoreBox->PlayScoreingEndAnimation();
}

APlayerCharacter* APlayerCharacterController::GetPlayer()
{
	if (!Player)
	{
		Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}

	return Player;
}

void APlayerCharacterController::NextRound(int _Round, float BreakTime)
{
	Round = _Round;
	RoundTimer = BreakTime;
	if (PerkSelectWidget)
	{
		if (Player)
		{
			Player->bCanControll = false;
			Player->bLMBInput = false;
		}
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
		PerkSelectWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void APlayerCharacterController::StartRound()
{
	if (PerkSelectWidget)
	{
		if (Player)
		{
			Player->bCanControll = true;
		}
		PerkSelectWidget->SetVisibility(ESlateVisibility::Hidden);
		bShowMouseCursor = false;
	}
}

void APlayerCharacterController::AddScore(int ScoreAmount)
{
	Score += ScoreAmount;

	if (WidgetScoreBox)
	{
		AddedScore += ScoreAmount;
		WidgetScoreBox->SetAddedScore(AddedScore);
		WidgetScoreBox->PlayScoreingBeginAnimation();
		GetWorldTimerManager().ClearTimer(ScoringResetTimerHandle);
		GetWorldTimerManager().SetTimer(ScoringResetTimerHandle, this, &APlayerCharacterController::ScoringReset, 3.f);
	}
}

void APlayerCharacterController::EnemyKill(const AEnemyCharacter* const Enemy)
{
	if (!Enemy || !IsValid(Enemy)) return;

	AddScore(Enemy->Score);
}