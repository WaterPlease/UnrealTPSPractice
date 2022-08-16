// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "UObject/ConstructorHelpers.h"
#include "PlayerCharacter.h"

UCustomGameUserSettings::UCustomGameUserSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Sensitivity = 1.f;
	MasterVolume = 100.f;

	PlayerName = FString("PLAYER");
}

UCustomGameUserSettings* UCustomGameUserSettings::GetUserSettings()
{
	return Cast<UCustomGameUserSettings>(UGameUserSettings::GetGameUserSettings());
}

void UCustomGameUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
	Super::ApplySettings(bCheckForCommandLineOverrides);
	
	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player)
	{
		Player->CameraSensitivity = Sensitivity;
	}
}

void UCustomGameUserSettings::SetPlayerName(FString Value)
{
	PlayerName = Value;
}

FString UCustomGameUserSettings::GetPlayerName() const
{
	return PlayerName;
}

void UCustomGameUserSettings::SetSensitivity(float Value)
{
	Sensitivity = FMath::Clamp(Value,MIN_SENSITIVITY,MAX_SENSITIVITY);
}

float UCustomGameUserSettings::GetSensitivity() const
{
	return Sensitivity;
}

void UCustomGameUserSettings::SetMasterVolume(float Value)
{
	MasterVolume = FMath::Clamp(Value,MIN_SOUNDVOLUME,MAX_SOUNDVOLUME);
}

float UCustomGameUserSettings::GetMasterVolume() const
{
	return MasterVolume;
}
