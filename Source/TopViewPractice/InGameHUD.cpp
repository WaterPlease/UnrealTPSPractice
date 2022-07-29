// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameHUD.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameUserSettings.h"

AInGameHUD::AInGameHUD()
{
	CrosshairLineWidth = 1.f;
	CrosshairLineLength = 10.f;
	HitmarkerLineWidth = 1.f;
	HitmarkerLineLength = 10.f;
	CrosshairLineColor = FLinearColor::Green;
	HitmarkerLineColor = FLinearColor::White;

	CrosshairLineOffset = 10.f;
	HitmarkerLineOffset = 10.f;

	CrosshairCenterSize = 5.f;
	CrosshairCursorColor = FLinearColor::White;

	bHitmarkerDisplay = false;
}

void AInGameHUD::BeginPlay()
{
	Super::BeginPlay();

}


/** Override for drawhud event */
void AInGameHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawCrosshair();

	if (bHitmarkerDisplay)
		DrawHitmarker();
}

void AInGameHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInGameHUD::DrawCrosshair()
{
	FVector2D Result;

	if (!(GEngine && GEngine->GameViewport)) return;


	GEngine->GameViewport->GetViewportSize(Result);
	FVector2D CenterPosition = Result * 0.5f;
	
	AHUD::DrawLine(
		CenterPosition.X + CrosshairLineOffset,
		CenterPosition.Y,
		CenterPosition.X + CrosshairLineOffset + CrosshairLineLength,
		CenterPosition.Y,
		CrosshairLineColor,
		CrosshairLineWidth
	);
	AHUD::DrawLine(
		CenterPosition.X,
		CenterPosition.Y + CrosshairLineOffset,
		CenterPosition.X,
		CenterPosition.Y + CrosshairLineOffset + CrosshairLineLength,
		CrosshairLineColor,
		CrosshairLineWidth
	);
	AHUD::DrawLine(
		CenterPosition.X - CrosshairLineOffset,
		CenterPosition.Y,
		CenterPosition.X - CrosshairLineOffset - CrosshairLineLength,
		CenterPosition.Y,
		CrosshairLineColor,
		CrosshairLineWidth
	);
	AHUD::DrawLine(
		CenterPosition.X,
		CenterPosition.Y - CrosshairLineOffset,
		CenterPosition.X,
		CenterPosition.Y - CrosshairLineOffset - CrosshairLineLength,
		CrosshairLineColor,
		CrosshairLineWidth
	);

	FVector2D CursorPosition = CenterPosition + CursorOffset;

	AHUD::DrawTexture(
		CrosshairCenterTexture,
		CursorPosition.X - CrosshairCenterSize * 0.5f,
		CursorPosition.Y - CrosshairCenterSize * 0.5f,
		CrosshairCenterSize, CrosshairCenterSize,
		0.f, 0.f, 1.f, 1.f, CrosshairCursorColor
	);
}

void AInGameHUD::DrawHitmarker()
{
	FVector2D Result;

	if (!(GEngine && GEngine->GameViewport)) return;


	GEngine->GameViewport->GetViewportSize(Result);
	FVector2D CenterPosition = Result * 0.5f;

	AHUD::DrawLine(
		CenterPosition.X + HitmarkerLineOffset,
		CenterPosition.Y + HitmarkerLineOffset,
		CenterPosition.X + HitmarkerLineOffset + HitmarkerLineLength,
		CenterPosition.Y + HitmarkerLineOffset + HitmarkerLineLength,
		HitmarkerLineColor,
		HitmarkerLineWidth
	);
	AHUD::DrawLine(
		CenterPosition.X - HitmarkerLineOffset,
		CenterPosition.Y + HitmarkerLineOffset,
		CenterPosition.X - HitmarkerLineOffset - HitmarkerLineLength,
		CenterPosition.Y + HitmarkerLineOffset + HitmarkerLineLength,
		HitmarkerLineColor,
		HitmarkerLineWidth
	);
	AHUD::DrawLine(
		CenterPosition.X - HitmarkerLineOffset,
		CenterPosition.Y - HitmarkerLineOffset,
		CenterPosition.X - HitmarkerLineOffset - HitmarkerLineLength,
		CenterPosition.Y - HitmarkerLineOffset - HitmarkerLineLength,
		HitmarkerLineColor,
		HitmarkerLineWidth
	);
	AHUD::DrawLine(
		CenterPosition.X + HitmarkerLineOffset,
		CenterPosition.Y - HitmarkerLineOffset,
		CenterPosition.X + HitmarkerLineOffset + HitmarkerLineLength,
		CenterPosition.Y - HitmarkerLineOffset - HitmarkerLineLength,
		HitmarkerLineColor,
		HitmarkerLineWidth
	);
}