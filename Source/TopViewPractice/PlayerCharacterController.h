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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI | Screen")
	TSubclassOf<UUserWidget> PerkSelectWidgetAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI | Screen")
	UUserWidget* PerkSelectWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI | Screen")
	class UScoringBox* WidgetScoreBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI | Data")
	int Score;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI | Data")
	int Round;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI | Data")
	float RoundTimer;


	class AInGameHUD* InGameHUD;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	class APlayerCharacter* Player;

	int AddedScore;

	FTimerHandle ScoringResetTimerHandle;
	UFUNCTION()
	void ScoringReset();

public:
	APlayerCharacter* GetPlayer();

	void NextRound(int _Round, float BreakTime);

	UFUNCTION(BlueprintCallable)
	void StartRound();

	UFUNCTION(BlueprintCallable)
	void AddScore(int ScoreAmount);

	UFUNCTION(BlueprintCallable)
	void EnemyKill(const class AEnemyCharacter* const Enemy);
};
