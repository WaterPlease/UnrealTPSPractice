// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class TOPVIEWPRACTICE_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerCharacterController();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI | Screen")
	TSubclassOf<UUserWidget> InGameScreenWidgetAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI | Screen")
	UUserWidget* InGameScreenWidget;

	class AInGameHUD* InGameHUD;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	class APlayerCharacter* Player;

	APlayerCharacter* GetPlayer();
};
