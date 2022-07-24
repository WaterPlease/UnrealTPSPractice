// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopViewPracticeGameMode.h"
#include "TopViewPracticePlayerController.h"
#include "TopViewPracticeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATopViewPracticeGameMode::ATopViewPracticeGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATopViewPracticePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}