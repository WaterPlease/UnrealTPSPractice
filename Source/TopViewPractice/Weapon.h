// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class TOPVIEWPRACTICE_API AWeapon : public AItem
{
	GENERATED_BODY()
		
public:
	// Initalization
	AWeapon();

	/**
	* Components
	*/

	/**
	* Parameter
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	class USkeletalMesh* WeaponMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FTransform WeaponRelativeTransform;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FText WeaponName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Particle")
	class UParticleSystem* MuzzleParticleSystem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Sound")
	class USoundCue* FireSoundCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Stats")
	TSubclassOf<class ABaseBullet> BulletType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Stats")
	bool bAutomatic;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Stats")
	float BaseDamage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Stats")
	float BaseGunRPM;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon | Stats")
	uint8 RoundCapacity;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Stats")
	float BaseMinSpread;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Stats")
	float BaseMaxSpread;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Stats")
	float BaseSpreadIncreaseSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Stats")
	float BaseSpreadRestoreSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Stats")
	float BaseVerticalRecoil;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Stats")
	float BaseHorizontalRecoil;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Stats")
	float BaseRecoilDamping;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon | Stats")
	FVector MuzzleSize3D;
	
private:

public:
	void Fire(const FVector& Location, const FVector& Direction, bool bShotByPlayer = true);

	virtual void OnItemSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnItemSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};
