// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseGrenade.generated.h"

UCLASS()
class TOPVIEWPRACTICE_API ABaseGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseGrenade();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	class USphereComponent* Sphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade")
	class USphereComponent* EffectSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade")
	TSubclassOf<UDamageType> DamageType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade")
	class UParticleSystem* DetonationParticle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade")
	float DetonationDelay;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade")
	float Damage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade")
	float ImpulseCharacterStrength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade")
	float ImpulsePropStrength;

	FTimerHandle DetonationTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Detonate();
};
