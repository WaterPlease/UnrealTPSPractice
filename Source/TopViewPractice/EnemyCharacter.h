// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UENUM()
enum class EEnemyActionState : uint8
{
	EEA_Idle UMETA(DisplayName = "Idle"),
	EEA_Chase UMETA(DisplayName = "Chase"),
	EEA_Attack UMETA(DisplayName = "Attack"),


	EEA_MAX UMETA(DisplayName="DefaultMax")
};

UCLASS()
class TOPVIEWPRACTICE_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	// Enemy character state
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy | State")
	EEnemyActionState EnemyActionState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy | State")
	bool bCanAttack;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy | State")
	bool bWaitAttack;

	// Enemy character Stat
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy | State")
	float AttackDelay;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	* User Function Declaration
	*/

	// Attack Sphere Overlap Handler
	UFUNCTION()
	void AttackSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void AttackSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void AttackCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void AttackCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void ActivateAttackCollision();
	UFUNCTION(BlueprintCallable)
	void DeactivateAttackCollision();
	UFUNCTION(BlueprintCallable)
	void AttackDone();
	/**
	* Component/Actor Get/Set function
	*/
	class APlayerCharacter* GetPlayerCharacter();

	void WaitAttackDone() { bWaitAttack = false; }

private:
	/**
	* User Private Field Declaration
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy | AI", meta = (AllowPrivateAccess = "true"))
	class AAIController* AIController;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy | AI", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* AttackSphere;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy | AI", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* AttackCollision;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy | AI", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* CombatMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy | Combat", meta = (AllowPrivateAccess = "true"))
	class USoundCue* AttackSoundCue;


	APlayerCharacter* PlayerCharacter;


	/**
	* User Private Function Declaration
	*/
	void ChasePlayer();
	void Attack();

	FTimerHandle AttackTimerHandle;
};
