// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class TOPVIEWPRACTICE_API AInGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	AInGameHUD();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI | CrossHair")
	UTexture* CrosshairCenterTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI | CrossHair")
	float CrosshairLineWidth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI | CrossHair")
	float CrosshairLineLength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI | CrossHair")
	float HitmarkerLineWidth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI | CrossHair")
	float HitmarkerLineLength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI | CrossHair")
	FLinearColor CrosshairLineColor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI | CrossHair")
	FLinearColor HitmarkerLineColor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI | CrossHair")
	float CrosshairCenterSize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI | CrossHair")
	FLinearColor CrosshairCursorColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI | CrossHair")
	float CrosshairLineOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI | CrossHair")
	float HitmarkerLineOffset;

	bool bHitmarkerDisplay;

	FVector2D CursorOffset;

	virtual void DrawHUD() override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	void DrawCrosshair();
	void DrawHitmarker();
};
