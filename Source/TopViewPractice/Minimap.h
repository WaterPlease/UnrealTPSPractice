// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Minimap.generated.h"

UCLASS()
class TOPVIEWPRACTICE_API AMinimap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMinimap();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	class USceneCaptureComponent2D* SceneCaptureComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	float CaptureHeight;

	class APlayerCharacter* Player;

	FTimerHandle AttachmentTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	APlayerCharacter* GetPlayer();
	void AttachToPlayer();
};
