// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseBullet.generated.h"

UCLASS()
class TOPVIEWPRACTICE_API ABaseBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseBullet();

	/**
	* Components
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USphereComponent* BulletHead;

	class UProjectileMovementComponent* ProjectileMovementComponent;

	/**
	* Parameters
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet | stats")
	TSubclassOf<UDamageType> DamageType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Bullet | stats")
	float LifeTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet | stats")
	float InitialSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet | stats")
	uint8 RemainBounce;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet | Effect")
	class UParticleSystem* SmokeParticleSystem;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet | Effect")
	TSubclassOf<class ADecalActor> BulletDecalAsset;

	float Damage;

	AActor* Shooter;
	class AController* Instigator;

	bool bShotByPlayer;

	FTimerHandle DestroyTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	void LauchBullet(const FVector& Direction);

	void DestroyBullet();

	UFUNCTION()
	void OnBulletHeadOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnBulletHeadOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnBulletHeadHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
