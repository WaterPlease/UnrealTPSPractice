// Fill out your copyright notice in the Description page of Project Settings.


#include "Minimap.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMinimap::AMinimap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(FName("SceneCaptureComponent"));
	SceneCaptureComponent->SetupAttachment(GetRootComponent());

	CaptureHeight = 1000.f;
}

// Called when the game starts or when spawned
void AMinimap::BeginPlay()
{
	Super::BeginPlay();
	GetPlayer();

	GetWorldTimerManager().SetTimer(AttachmentTimerHandle, this, &AMinimap::AttachToPlayer, 0.2f);
}

// Called every frame
void AMinimap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetPlayer())
	{
		//FRotator Rotation = Player->CameraBoom->GetComponentRotation();
		FVector ForwardVector = Player->CameraBoom->GetForwardVector();
		ForwardVector.Z = 0.f;
		FRotator Rotation = ForwardVector.Rotation();
		//Rotation.Pitch = -90.f;
		SceneCaptureComponent->SetWorldRotation(Rotation);
		SceneCaptureComponent->AddRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	}
}

APlayerCharacter* AMinimap::GetPlayer()
{
	if (!Player)
	{
		Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}

	return Player;
}

void AMinimap::AttachToPlayer()
{
	if (GetPlayer())
	{
		FAttachmentTransformRules AttachRule(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::SnapToTarget,true);
		SceneCaptureComponent->AttachToComponent(Player->GetRootComponent(),FAttachmentTransformRules::SnapToTargetIncludingScale);
		SceneCaptureComponent->SetRelativeLocation(FVector(0.f, 0.f, CaptureHeight));
		SceneCaptureComponent->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	}
}
