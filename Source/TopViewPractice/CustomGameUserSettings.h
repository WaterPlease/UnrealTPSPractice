// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "CustomGameUserSettings.generated.h"

/**
 * 
 */
UCLASS(config = GameUserSettings)
class TOPVIEWPRACTICE_API UCustomGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
public:
	UCustomGameUserSettings(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable)
	static UCustomGameUserSettings* GetUserSettings();

	virtual void ApplySettings(bool bCheckForCommandLineOverrides) override;
	UFUNCTION(BlueprintCallable, Category = "Settings | User")
	void SetPlayerName(FString Value);
	UFUNCTION(BlueprintPure, Category = "Settings | User")
	FString GetPlayerName() const;

	UFUNCTION(BlueprintCallable, Category = "Settings | Control")
	void SetSensitivity(float Value);
	UFUNCTION(BlueprintPure, Category = "Settings | Control")
	float GetSensitivity() const;

	UFUNCTION(BlueprintCallable, Category = "Settings | Sound")
	void SetMasterVolume(float Value);
	UFUNCTION(BlueprintPure, Category = "Settings | Sound")
	float GetMasterVolume() const;

	const float MIN_SENSITIVITY = 0.1f;
	const float MAX_SENSITIVITY = 5.f;

	const float MIN_SOUNDVOLUME = 0.0f;
	const float MAX_SOUNDVOLUME = 100.f;
protected:
	UPROPERTY(config)
	float Sensitivity;

	UPROPERTY(config)
	float MasterVolume;

	UPROPERTY(config)
	FString PlayerName;
};
