// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UENUM()
enum class EPlayerActionState : uint8
{
	EPA_Idle UMETA(DisplayName = "Idle"),
	EPA_Throwing UMETA(DisplayName = "Throwing"),
	EPA_Reloading UMETA(DisplayName = "Reloading"),
	EPA_Diving UMETA(DisplayName = "Diving"),
	EPA_Dead UMETA(DisplayName = "Dead"),

	EPA_MAX UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class TOPVIEWPRACTICE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	/**
	* Components
	*/
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;
	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Cursor", meta = (AllowPrivateAccess = "true"))
	UDecalComponent* CursorToWorld;
	/** A decal that projects to the cursor location. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | UI", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* CombatUI;


	/**
	* Weapon components from AWeapon Actor
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Combat")
	class USkeletalMeshComponent* WeaponMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Combat")
	class USoundCue* GunFireSoundCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Combat")
	class USoundCue* ThrowingSoundCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Combat")
	class USoundCue* MagInSoundCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Combat")
	class USoundCue* MagOutSoundCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Combat")
	class USoundCue* RackingBoltSoundCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Combat")
	class USoundCue* EmptyShotSoundCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Combat")
	class UParticleSystem* MuzzleParticleSystem;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Combat")
	class UBoxComponent* MuzzleUpTriggerBox;
	
	
	/** Animation Montage for Character's Action*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Combat", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* FireMontage;

	/**
	* Input State
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input State")
	bool bLMBInput;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input State")
	bool bRMBInput;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input State")
	bool bRunInput;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input State")
	bool bCrouchInput;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input State")
	bool bGrenadeInput;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input State")
	bool bReloadInput;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input State")
	bool bDodgeInput;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input State")
	bool bInteractionInput;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input State")
	bool bCameraRotationInput;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input State")
	bool bMuzzleUp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input State")
	bool bRunning;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input State")
	bool bCrouch;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input State")
	bool bTryFire;

	// Player character state
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character | State")
	EPlayerActionState PlayerActionState;

	/**
	* Fields for configuration
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Camera")
	float CameraDistance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Camera")
	float CameraMaxDistance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Camera")
	float CameraMinDistance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Camera")
	float CameraADSDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Camera")
	float CameraSensitivity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Camera")
	uint8 ZoomLevel;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Stats")
	float BaseGunRPM;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Stats")
	float GunRPMMultiplier;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character | Stats")
	uint8 RoundCapacity;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character | Stats")
	uint8 RoundRemain;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character | Stats")
	float MaxHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character | Stats")
	float Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character | Stats")
	float HealthRegenRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character | Stats")
	float HealthRegenDelay;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character | Stats")
	float MaxStamina;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character | Stats")
	float Stamina;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character | Stats")
	float StaminaRegenRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character | Stats")
	float DodgeStamina;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character | Stats")
	float JumpStamina;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character | Stats")
	float WalkSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character | Stats")
	float RunSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character | Stats")
	float ADSSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character | Stats")
	float DiveAcceleration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character | Stats")
	float DiveDuration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character | Stats")
	float CharacterTurnSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character | Stats")
	float CharacterAimTurnSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character | Stats")
	float TimeToMoveMuzzle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | IK")
	bool bIKRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | IK")
	float IKRotaionStrength;

	float TimeMuzzleMoving;
	bool bCanAim;
	bool bHeal;
	float SpinePitch;
	float TargetSpeed;
	FRotator CamRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character | Weapon")
	class AWeapon* EquippedWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character | Weapon")
	class AWeapon* WeaponToEquipped;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | UI")
	FRotator CombatUIIdleRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | UI")
	FRotator CombatUIADSRotation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	/**
	* Engine Function
	*/
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	* Input Handler Function
	*/
	UFUNCTION()
	void InputVertical(float Value);
	UFUNCTION()
	void InputHorizontal(float Value);
	UFUNCTION()
	void Zoom(float Value);
	UFUNCTION()
	void HorizontalTurn(float Value);
	UFUNCTION()
	void VerticalTurn(float Value);

	
	UFUNCTION()
	void LMBDown();
	UFUNCTION()
	void LMBUp();

	UFUNCTION()
	void RMBDown();
	UFUNCTION()
	void RMBUp();

	UFUNCTION()
	void RunDown();
	UFUNCTION()
	void RunUp();

	UFUNCTION()
	void CrouchDown();
	UFUNCTION()
	void CrouchUp();
	
	virtual void Jump() override;

	UFUNCTION()
	void GrenadeDown();
	UFUNCTION()
	void GrenadeUp();

	UFUNCTION()
	void ReloadDown();
	UFUNCTION()
	void ReloadUp();

	UFUNCTION()
	void InteractionDown();
	UFUNCTION()
	void InteractionUp();

	UFUNCTION()
	void DodgeDown();
	UFUNCTION()
	void DodgeUp();

	UFUNCTION()
	void CameraRotationDown();
	UFUNCTION()
	void CameraRotationUp();

	/**
	* User Function Declaration
	*/
	bool CanMove();
	
	UFUNCTION(BlueprintCallable)
	void Reload();

	UFUNCTION(BlueprintCallable)
	void DiveDone();

	void EquipWeapon(class AWeapon* Weapon);
	void Die();

	// Muzzle Up trigger handler
	UFUNCTION()
	void OnMuzzleTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnMuzzleTriggerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


	/**
	* Component Get/Set function
	*/
	UFUNCTION(BlueprintCallable)
	class UCameraComponent* GetCameraComponent() { return CameraComponent; }
	UFUNCTION(BlueprintCallable)
	class USpringArmComponent* GetCameraBoom() { return CameraBoom; }
	UFUNCTION(BlueprintCallable)
	class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }
	
	UFUNCTION(BlueprintCallable)
	float GetCameraDistance();
	UFUNCTION(BlueprintCallable)
	void SetCameraDistance(float Value);

private:
	/**
	* Private Variable Declaration
	*/
	FRotator CharcterRotation;
	FRotator DiveRotation;

	FVector DiveDirection;

	float TempCameraDistance;

	FTimerHandle FireTimerHandle;
	TArray<AActor*> MuzzleColliderArray;

	FRotator CombatUIRotation;

	/**
	* User Private Function Declaration
	*/
	void UpdateCharacterLook();
	void UpdateCurosorTransform();

	void RestorCharacterSpeed();

	void Fire();
};
