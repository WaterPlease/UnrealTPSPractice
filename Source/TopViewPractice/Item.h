// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS(meta=(BlueprintSpawnableComponent))
class TOPVIEWPRACTICE_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	class USphereComponent* ItemSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	class UParticleSystemComponent* ItemParticleSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	class UParticleSystem* IdleItemParticleSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	class UParticleSystem* HighlightenItemParticleSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	bool bHighlighten;

	FORCEINLINE class UStaticMeshComponent* GetMesh() { return Mesh; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	bool ToggleHighlight();

	UFUNCTION()
	virtual void OnItemSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnItemSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
