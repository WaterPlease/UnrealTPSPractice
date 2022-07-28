// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

enum EBodypart : uint8
{
	Head = 0,
	Hand_L = 1,
	LowArm_L = 2,
	UpArm_L = 3,
	Hand_L = 4,
	LowArm_L = 5,
	UpArm_L = 6,
};

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


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy | AI")
	class AAIController* AIController;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy | AI")
	class USphereComponent* AttackSphere;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy | AI")
	class USphereComponent* LookAtSphere;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy | AI")
	class UBoxComponent* AttackCollisionA;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy | AI")
	class UBoxComponent* AttackCollisionB;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy | AI")
	class UAnimMontage* CombatMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy | Combat")
	class USoundCue* AttackSoundCue;

	// Enemy character state
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy | State")
	EEnemyActionState EnemyActionState;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy | State")
	uint8 NumAttackType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy | State")
	bool bCanAttack;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy | State")
	bool bWaitAttack;

	// Enemy character Stat
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy | State")
	float AttackDelay;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy | UI")
	class UStaticMeshComponent* RadarPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy | Combat")
	class UCapsuleComponent* Bodyparts[15];
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
	UFUNCTION()
	void LookAtSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void LookAtSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void ActivateAttackCollision(int idx);
	UFUNCTION(BlueprintCallable)
	void DeactivateAttackCollision(int idx);
	UFUNCTION(BlueprintCallable)
	void AttackDone();
	/**
	* Component/Actor Get/Set function
	*/
	class APlayerCharacter* GetPlayerCharacter();

	void WaitAttackDone() { bWaitAttack = false; }

private:
	APlayerCharacter* PlayerCharacter;

	TArray<UBoxComponent*> AttackCollisions;

	/**
	* User Private Function Declaration
	*/
	void LookAtPlayer(float DeltaTime);
	void ChasePlayer();
	void Attack();

	bool bTryLookAt;

	FTimerHandle AttackTimerHandle;
};
