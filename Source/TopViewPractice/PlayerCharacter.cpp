// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/HUD.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Weapon.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/GameUserSettings.h"
#include "Math/UnrealMathUtility.h"
#include "BaseGrenade.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Prevent rotation of character by camera
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Character movement Configuration
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// CameraBoom Configuration
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 5.f;
	CameraBoom->CameraLagMaxDistance = 30.f;

	// Camera Configuration
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	// Combat UI Configuration
	CombatUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("CombatUI"));
	//CombatUI->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CombatUI->SetupAttachment(CameraComponent);
	CombatUI->bHiddenInSceneCapture = true;


	/////////////////////////////////////////////////////////////////////////////
	// From : Unreal template(TopDown) code.
	// 
	// Create a decal in the world to show the cursor's location
	//CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	//CursorToWorld->SetupAttachment(RootComponent);
	//static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	//if (DecalMaterialAsset.Succeeded())
	//{
	//	CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	//}
	//CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	//CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	/////////////////////////////////////////////////////////////////////////////

	// Weapon Mesh
	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
	//WeaponMeshComponent->AttachTo(GetMesh(), FName("LeftHandSocket"));
	WeaponMeshComponent->AttachTo(GetMesh(), FName("RightHandSocket"));
	//WeaponMeshComponent->AttachTo(GetRootComponent());

	// Muzzle Up Triggerbox
	MuzzleUpTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MuzzleUpTriggerBox"));
	MuzzleUpTriggerBox->SetupAttachment(GetMesh());

	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCue(
		TEXT("SoundCue'/Game/PlayerCharacter/Sound/HitmarkerSound_Cue.HitmarkerSound_Cue'")
	);
	HitSoundCue = SoundCue.Object;

	// Input State Initialization
	bLMBInput = false;
	bRMBInput = false;
	bCrouchInput = false;
	bInteractionInput = false;
	bDodgeInput = false;
	bMuzzleUp = false;
	bRunning = false;
	bCrouch = false;
	bTryFire = false;
	bCanAim = false;
	bHeal = true;
	bLMBLook = false;

	PlayerActionState = EPlayerActionState::EPA_Idle;

	// User Field Initialization
	CameraDistance = 800.f;
	CameraMaxDistance = 800.f;
	CameraMinDistance = 100.f;
	CameraADSDistance = 50.f;
	CameraSensitivity = 1.f;
	ZoomLevel = 3;
	SetCameraDistance(CameraDistance);

	bAutomatic = false;
	BaseGunRPM = 650.f;
	GunRPMMultiplier = 1.f;
	RoundCapacity = 30;
	RoundRemain = 31;
	GrenadeRemain = 0;
	ThrowSpeed = 1600.f;
	Spread = 0.f;
	BaseMinSpread = 0.f;
	BaseMaxSpread = 0.f;
	BaseSpreadIncreaseSpeed = 0.2;
	BaseSpreadRestoreSpeed = 5.f;
	BaseVerticalRecoil = 0.f;
	BaseHorizontalRecoil = 0.f;
	MaxHealth = 100.f;
	Health = 100.f;
	HealthRegenRate = 10.f;
	HealthRegenDelay = 5.f;
	MaxStamina = 100.f;
	Stamina = 100.f;
	StaminaRegenRate = 20.f;
	WalkSpeed = 200.f;
	RunSpeed = 600.f;
	ADSSpeed = 100.f;
	DiveAcceleration = 3.f;
	DiveDuration = 1.f;
	CharacterTurnSpeed = 15.f;
	CharacterAimTurnSpeed = 15.f;
	DodgeStamina = 50.f;
	JumpStamina = 25.f;

	bIKRotation = true;
	IKRotaionStrength = 1.2f;

	TimeMuzzleMoving = 0.f;
	TimeToMoveMuzzle = 1.0f;
	
	SpinePitch = 0.f;

	CharcterRotation = FRotator(0.f);
	CombatUIADSRotation = FRotator(0.0f, 180.f, 0.0);
	CombatUIIdleRotation = FRotator(0.0f, 230.f, 0.0);
	CrosshairOffset = 10.f;
	bHitmarkerDisplay = false;
	MaxHitmarkerLifeTime = 1.f;
	CursorLineTraceDistance = 10000.f;
	
	CursorOffset = FVector2D(0.f);
	RecoilVelocity = FRotator(0.f);


 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// CameraBoom Configuration
	SetCameraDistance(CameraDistance);

	// Muzzle up trigger event
	MuzzleUpTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnMuzzleTriggerOverlapBegin);
	MuzzleUpTriggerBox->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnMuzzleTriggerOverlapEnd);
	MuzzleUpTriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MuzzleUpTriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	MuzzleUpTriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	TargetSpeed = WalkSpeed;

	CombatUIRotation = CombatUIIdleRotation;

	Spread = BaseMinSpread;

	GrenadeRemain = 1;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Find Cursor position in world space and Character Look cursor.
	UpdateCurosorTransform();
	UpdateCharacterLook();

	// Tick function by Player's action state
	switch (PlayerActionState)
	{
	case EPlayerActionState::EPA_Idle :
		if (bHeal)
		{
			Health = FMath::Min(MaxHealth, Health + DeltaTime * HealthRegenRate);
		}
		Stamina = FMath::Min(MaxStamina, Stamina + DeltaTime * StaminaRegenRate);
		break;
	case EPlayerActionState::EPA_Diving :
		AddMovementInput(DiveDirection, 1.0f);
		break;
	default :
		break;
	}

	// Muzzle up check & smooth animation
	if (bMuzzleUp)
		TimeMuzzleMoving += DeltaTime;
	else
		TimeMuzzleMoving -= DeltaTime;
	TimeMuzzleMoving = FMath::Min(FMath::Max(TimeMuzzleMoving,0.f), TimeToMoveMuzzle);


	// Get Pithch angle of spine to make character aim to cursor
	if ((bRMBInput || bTryFire) && bCanAim)
	{
		FVector NeckToCursor = GetMesh()->GetSocketLocation("Neck") - CursorLocation;
		float DistOnXY = FMath::Sqrt(NeckToCursor.X * NeckToCursor.X + NeckToCursor.Y * NeckToCursor.Y);
		SpinePitch = FMath::RadiansToDegrees(FMath::Atan2(NeckToCursor.Z, DistOnXY));
	}	
	else if (bRMBInput || bTryFire)
	{
		FVector FrontVector = -GetCameraComponent()->GetForwardVector();
		float DistOnXY = FMath::Sqrt(FrontVector.X * FrontVector.X + FrontVector.Y * FrontVector.Y);
		SpinePitch = FMath::RadiansToDegrees(FMath::Atan2(FrontVector.Z, DistOnXY));
	}

	// Camera rotaion
	RecoilDampingTime += DeltaTime;
	FRotator RecoilRotation = DeltaTime * RecoilVelocity * FMath::Exp(-RecoilDampingTime * BaseRecoilDamping);
	if (bCrouch)
		RecoilRotation *= 0.5f;
	CamRotation += RecoilRotation;
	CamRotation.Pitch = FMath::Clamp<float>(CamRotation.Pitch, -80.f, 80.f);
	CameraBoom->SetWorldRotation(CamRotation);
	
	// Smooth camera
	float TempDist = CameraBoom->TargetArmLength;
	float TempRot = CameraBoom->GetRelativeRotation().Pitch;
	if (!FMath::IsNearlyEqual(TempDist, CameraDistance))
	{
		TempDist = FMath::FInterpTo(TempDist, CameraDistance, DeltaTime, CameraBoom->CameraLagSpeed);

		CameraBoom->TargetArmLength = TempDist;
	}

	float TempSpeed = GetCharacterMovement()->MaxWalkSpeed;
	if (!FMath::IsNearlyEqual(TempSpeed, TargetSpeed))
	{
		TempSpeed = FMath::FInterpTo(TempSpeed, TargetSpeed, DeltaTime, CameraBoom->CameraLagSpeed);
		GetCharacterMovement()->MaxWalkSpeed = TempSpeed;
		GetCharacterMovement()->MaxWalkSpeedCrouched = TempSpeed * 0.5f;
	}

	// Smooth Combat UI Rotation
	FRotator CurrentUIRotation = CombatUI->GetRelativeRotation();
	if (!CurrentUIRotation.Equals(CombatUIRotation))
	{
		CurrentUIRotation = FMath::RInterpTo(
			CurrentUIRotation,
			CombatUIRotation,
			DeltaTime,
			5.f
		);
		CombatUI->SetRelativeRotation(CurrentUIRotation);
	}

	// Weapon Stat tick
	if (!bTryFire && !FMath::IsNearlyEqual(Spread, BaseMinSpread))
	{
		Spread = FMath::FInterpTo(
			Spread,
			BaseMinSpread,
			DeltaTime,
			BaseSpreadRestoreSpeed
		);
	}
	TimeAfterShot += DeltaTime;
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("LMB"), EInputEvent::IE_Pressed, this, &APlayerCharacter::LMBDown);
	PlayerInputComponent->BindAction(TEXT("LMB"), EInputEvent::IE_Released, this, &APlayerCharacter::LMBUp);

	PlayerInputComponent->BindAction(TEXT("RMB"), EInputEvent::IE_Pressed, this, &APlayerCharacter::RMBDown);
	PlayerInputComponent->BindAction(TEXT("RMB"), EInputEvent::IE_Released, this, &APlayerCharacter::RMBUp);

	PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Pressed, this, &APlayerCharacter::RunDown);
	PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Released, this, &APlayerCharacter::RunUp);
	
	PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this, &APlayerCharacter::CrouchDown);
	PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Released, this, &APlayerCharacter::CrouchUp);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Jump);
	//PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &APlayerCharacter::CrouchUp);

	PlayerInputComponent->BindAction(TEXT("Grenade"), EInputEvent::IE_Pressed, this, &APlayerCharacter::GrenadeDown);
	PlayerInputComponent->BindAction(TEXT("Grenade"), EInputEvent::IE_Released, this, &APlayerCharacter::GrenadeUp);

	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &APlayerCharacter::ReloadDown);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Released, this, &APlayerCharacter::ReloadUp);

	PlayerInputComponent->BindAction(TEXT("Dodge"), EInputEvent::IE_Pressed, this, &APlayerCharacter::DodgeDown);
	PlayerInputComponent->BindAction(TEXT("Dodge"), EInputEvent::IE_Released, this, &APlayerCharacter::DodgeUp);

	PlayerInputComponent->BindAction(TEXT("Interaction"), EInputEvent::IE_Pressed, this, &APlayerCharacter::InteractionDown);
	PlayerInputComponent->BindAction(TEXT("Interaction"), EInputEvent::IE_Released, this, &APlayerCharacter::InteractionUp);
	
	PlayerInputComponent->BindAction(TEXT("CameraRotation"), EInputEvent::IE_Pressed, this, &APlayerCharacter::CameraRotationDown);
	PlayerInputComponent->BindAction(TEXT("CameraRotation"), EInputEvent::IE_Released, this, &APlayerCharacter::CameraRotationUp);

	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &APlayerCharacter::InputHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &APlayerCharacter::InputVertical);

	PlayerInputComponent->BindAxis(TEXT("Zoom"), this, &APlayerCharacter::Zoom);
	PlayerInputComponent->BindAxis(TEXT("HorizontalTurn"), this, &APlayerCharacter::HorizontalTurn);
	PlayerInputComponent->BindAxis(TEXT("VerticalTurn"), this, &APlayerCharacter::VerticalTurn);
}

// Vertical movement input handler
void APlayerCharacter::InputVertical(float Value)
{
	if (CanMove())
	{
		FVector FrontVector = GetCameraComponent()->GetForwardVector();
		FrontVector.Z = 0.f;
		FVector Direction = FrontVector.GetSafeNormal();
		AddMovementInput(Direction, Value);
	}
}

// Vertical movement input handler
void APlayerCharacter::InputHorizontal(float Value)
{
	if (CanMove())
	{
		FVector RightVector = GetCameraComponent()->GetRightVector();
		RightVector.Z = 0.f;
		FVector Direction = RightVector.GetSafeNormal();
		AddMovementInput(Direction, Value);
	}
}

// Zoom input handler
void APlayerCharacter::Zoom(float Value)
{
	if (!FMath::IsNearlyZero(Value) && !bRMBInput)
	{
		float DeltaDistance = (CameraMaxDistance - CameraMinDistance) / (float)ZoomLevel;

		CameraDistance = FMath::Clamp<float>(CameraDistance - DeltaDistance * Value, CameraMinDistance, CameraMaxDistance);
	}
}

// Horizontal turn (mouse drag) input handler
void APlayerCharacter::HorizontalTurn(float Value)
{
	CamRotation.Yaw += Value * CameraSensitivity;
}

// Horizontal turn (mouse drag) input handler
void APlayerCharacter::VerticalTurn(float Value)
{
	CamRotation.Pitch += Value * CameraSensitivity;
}

void APlayerCharacter::LMBDown()
{
	bLMBInput = true;
	bTryFire = true;
	bLMBLook = true;
	Fire();
}

void APlayerCharacter::LMBUp()
{
	bLMBInput = false;
	bTryFire = false;
	GetWorldTimerManager().ClearTimer(LMBLookTimerHandle);
	GetWorldTimerManager().SetTimer(LMBLookTimerHandle, this, &APlayerCharacter::ResetLMBLook, 2.0f);
}

void APlayerCharacter::RMBDown()
{
	bRMBInput = true;

	TempCameraDistance = CameraDistance;
	CameraDistance = CameraADSDistance;
	TargetSpeed = ADSSpeed;
	CombatUIRotation = CombatUIADSRotation;
}

void APlayerCharacter::RMBUp()
{
	bRMBInput = false;

	CameraDistance = TempCameraDistance;
	CombatUIRotation = CombatUIIdleRotation;
	RestorCharacterSpeed();
}

void APlayerCharacter::RunDown()
{
	bRunInput = true;

	if (!bRMBInput)
	{
		bRunning = !bRunning;
		RestorCharacterSpeed();
	}
}

void APlayerCharacter::RunUp()
{
	bRunInput = false;
}

void APlayerCharacter::CrouchDown()
{
	bCrouchInput = true;

	bCrouch = !bCrouch;
	RestorCharacterSpeed();

	if (bCrouch)
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(54.f);
		GetMesh()->AddRelativeLocation(FVector(0.f, 0.f, 34.f));
		MuzzleUpTriggerBox->AddRelativeLocation(FVector(0.f, 0.f, -34.f));
	}
	else
	{
		MuzzleUpTriggerBox->AddRelativeLocation(FVector(0.f, 0.f, 34.f));
		GetMesh()->AddRelativeLocation(FVector(0.f, 0.f, -34.f));
		GetCapsuleComponent()->SetCapsuleHalfHeight(88.f);
	}
}

void APlayerCharacter::CrouchUp()
{
	bCrouchInput = false;
}

void APlayerCharacter::Jump()
{
	if (Stamina > JumpStamina &&
		CanJump())
	{
		Stamina -= JumpStamina;
		Super::Jump();
	}
}

void APlayerCharacter::GrenadeDown()
{
	bGrenadeInput = true;

	ThrowGrenade();

	bLMBLook = true;
	GetWorldTimerManager().SetTimer(LMBLookTimerHandle, this, &APlayerCharacter::ResetLMBLook, 3.0f);
}

void APlayerCharacter::GrenadeUp()
{
	bGrenadeInput = false;
}

void APlayerCharacter::ReloadDown()
{
	bReloadInput = true;

	if (PlayerActionState != EPlayerActionState::EPA_Idle) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(FireMontage, 2.f);
		AnimInstance->Montage_JumpToSection(FName("Reload"), FireMontage);

		PlayerActionState = EPlayerActionState::EPA_Reloading;
	}
}

void APlayerCharacter::ReloadUp()
{
	bReloadInput = false;
}

void APlayerCharacter::InteractionDown()
{
	bInteractionInput = true;

	if (WeaponToEquipped)
	{
		EquipWeapon(WeaponToEquipped);
	}
}

void APlayerCharacter::InteractionUp()
{
	bInteractionInput = false;
}

void APlayerCharacter::DodgeDown()
{
	bDodgeInput = true;

	if (PlayerActionState == EPlayerActionState::EPA_Diving ||
		GetCharacterMovement()->IsFalling() ||
		Stamina < DodgeStamina) return;
	Stamina -= DodgeStamina;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Stop(0.1f, FireMontage);
		AnimInstance->Montage_Play(FireMontage, 2.37f / DiveDuration);
		AnimInstance->Montage_JumpToSection(FName("Dive"), FireMontage);


		// Find direction of diving
		// => Moving direction or character's looking direction if no movement
		FVector FrontVector = GetCameraComponent()->GetForwardVector();
		FrontVector.Z = 0.f;
		FrontVector = FrontVector.GetSafeNormal();
		FVector RightVector = GetCameraComponent()->GetRightVector();
		RightVector.Z = 0.f;
		RightVector = RightVector.GetSafeNormal();

		DiveDirection = FrontVector * GetInputAxisValue(FName("Vertical")) +
							 RightVector * GetInputAxisValue(FName("Horizontal"));
		if (DiveDirection.IsNearlyZero())
		{
			DiveDirection = CursorLocation - GetActorLocation();
			DiveDirection.Z = 0.f;
		}	
		else
		{
			DiveDirection = DiveDirection.GetSafeNormal();
		}

		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(FVector(0.f), DiveDirection);
		Rotation.Pitch = 0.f;
		Rotation.Roll = 0.f;
		Rotation.Yaw -= 90.f;
		GetMesh()->SetWorldRotation(Rotation);
		DiveRotation = Rotation;

		// Set moving speed
		TargetSpeed = RunSpeed * DiveAcceleration;

		PlayerActionState = EPlayerActionState::EPA_Diving;

		// Cancel other aciton.
		GetWorldTimerManager().ClearTimer(FireTimerHandle);
	}
}

void APlayerCharacter::DodgeUp()
{
	bDodgeInput = false;
}

void APlayerCharacter::CameraRotationDown()
{
	bCameraRotationInput = true;
}

void APlayerCharacter::CameraRotationUp()
{
	bCameraRotationInput = false;
}

bool APlayerCharacter::CanMove()
{
	return 
		PlayerActionState != EPlayerActionState::EPA_Diving &&
		PlayerActionState != EPlayerActionState::EPA_Dead;
}

void APlayerCharacter::Reload()
{
	if (RoundRemain == 0)
		RoundRemain = RoundCapacity;
	else
		RoundRemain = RoundCapacity + 1;
	UE_LOG(LogTemp, Warning, TEXT("Remain Round : %d"), RoundRemain);
}

void APlayerCharacter::DiveDone()
{
	PlayerActionState = EPlayerActionState::EPA_Idle;
	RestorCharacterSpeed();
}

void APlayerCharacter::SpawnGrenade()
{
	FVector LeftHandLocation = GetMesh()->GetSocketLocation("LeftHandSocket");

	ABaseGrenade* Grenade = GetWorld()->SpawnActor<ABaseGrenade>(GrenadeType,
		LeftHandLocation,
		FMath::VRand().Rotation());

	if (!Grenade)
	{
		// Failed to spawn grenade
		return;
	}

	GrenadeRemain -= 1;

	Grenade->Sphere->SetAllPhysicsLinearVelocity(
		ThrowSpeed * (CursorLocation - LeftHandLocation).GetSafeNormal()
	);
	Grenade->Sphere->SetAllPhysicsAngularVelocityInDegrees(
		FMath::VRand() * 720.f
	);
}

// Equip Weapon
void APlayerCharacter::EquipWeapon(AWeapon* Weapon)
{
	AWeapon* OldWeapon = EquippedWeapon;
	EquippedWeapon = Weapon;
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("RightHandSocket"));
	Weapon->GetMesh()->SetVisibility(false);
	Weapon->ItemParticleSystem->SetVisibility(false);
	WeaponMeshComponent->SetSkeletalMesh(Weapon->WeaponMesh);
	WeaponMeshComponent->SetRelativeTransform(Weapon->WeaponRelativeTransform);

	MuzzleUpTriggerBox->SetWorldLocation(WeaponMeshComponent->GetSocketLocation("MuzzleSocket"));
	MuzzleUpTriggerBox->SetWorldScale3D(Weapon->MuzzleSize3D);
	MuzzleUpTriggerBox->AddRelativeLocation(FVector(0.f, -MuzzleUpTriggerBox->GetScaledBoxExtent().Y, 0.f));

	GunFireSoundCue = Weapon->FireSoundCue;
	MuzzleParticleSystem = Weapon->MuzzleParticleSystem;

	bAutomatic = Weapon->bAutomatic;
	BaseGunRPM = Weapon->BaseGunRPM;
	RoundCapacity = Weapon->RoundCapacity;
	RoundRemain = RoundCapacity+1;
	BaseMinSpread = Weapon->BaseMinSpread;
	BaseMaxSpread = Weapon->BaseMaxSpread;
	BaseSpreadIncreaseSpeed = Weapon->BaseSpreadIncreaseSpeed;
	BaseSpreadRestoreSpeed = Weapon->BaseSpreadRestoreSpeed;
	BaseHorizontalRecoil = Weapon->BaseHorizontalRecoil;
	BaseRecoilDamping = Weapon->BaseRecoilDamping;
	BaseVerticalRecoil = Weapon->BaseVerticalRecoil;

	Spread = BaseMinSpread;

	if (OldWeapon)
	{
		OldWeapon->Destroy();
	}
}

void APlayerCharacter::Die()
{
	PlayerActionState = EPlayerActionState::EPA_Dead;
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
}

// Muzzle up event handler
void APlayerCharacter::OnMuzzleTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bMuzzleUp = true;
	
	if (MuzzleColliderArray.Num()==0)
		TimeMuzzleMoving = 0.f;

	MuzzleColliderArray.Add(OtherActor);

	UE_LOG(LogTemp, Warning, TEXT("Muzzle UP!"));
}

// Muzzle up event handler
void APlayerCharacter::OnMuzzleTriggerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (MuzzleColliderArray.Contains(OtherActor))
		MuzzleColliderArray.Remove(OtherActor);

	bMuzzleUp = MuzzleColliderArray.Num() > 0;

	if (MuzzleColliderArray.Num() == 0)
	{
		bMuzzleUp = false;
		UE_LOG(LogTemp, Warning, TEXT("Muzzle Down!"));
	}
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float EffectiveDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	bHeal = false;
	Health -= EffectiveDamage; 
	if (Health < 0.f)
	{
		Die();
	}
	else
	{
		GetWorldTimerManager().SetTimer(HealTimerHandle, this, &APlayerCharacter::SetHealable, HealthRegenDelay);
	}
	return EffectiveDamage;
}

void APlayerCharacter::SetHealable()
{
	bHeal = true;
}

void APlayerCharacter::ShowHitmarker()
{
	bHitmarkerDisplay = true;

	UGameplayStatics::PlaySound2D(GetWorld(),HitSoundCue);

	GetWorldTimerManager().ClearTimer(HitmarkerTimerHandle);
	GetWorldTimerManager().SetTimer(HitmarkerTimerHandle,this,&APlayerCharacter::HideHitmarker,MaxHitmarkerLifeTime);
}

void APlayerCharacter::HideHitmarker()
{
	bHitmarkerDisplay = false;
}

void APlayerCharacter::ResetCursorOffset()
{
	CursorOffset = FVector2D(0.f);
}

void APlayerCharacter::ResetRecoilVelocity()
{
	RecoilVelocity = FRotator(0.f);
}


float APlayerCharacter::GetCameraDistance()
{ 
	return CameraDistance;
}

void APlayerCharacter::SetCameraDistance(float Value)
{
	CameraDistance = Value;
}

// Smooth character look direction update
void APlayerCharacter::UpdateCharacterLook()
{
	FRotator Rotation;
	if (PlayerActionState == EPlayerActionState::EPA_Diving)
	{
		Rotation = DiveRotation;
	}
	else
	{
		FVector FrontVector = GetCameraComponent()->GetForwardVector();
		FVector RightVector = GetCameraComponent()->GetRightVector();

		FVector LookVector;
		//	UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CursorLocation);
		if (bRMBInput || bLMBLook)
		{
			LookVector = CursorLocation - GetActorLocation();//FrontVector;
		}
		else
		{
			LookVector = (GetInputAxisValue(FName("Vertical")) * FrontVector +
				GetInputAxisValue(FName("Horizontal")) * RightVector);
		}

		LookVector.Z = 0;

		if (!LookVector.IsNearlyZero())
		{
			Rotation = LookVector.Rotation();

			Rotation.Pitch = 0.f;
			Rotation.Roll = 0.f;
			Rotation.Yaw -= 90.f;
		}
		else
		{
			Rotation = CharcterRotation;
		}
	}

	CharcterRotation = FMath::RInterpTo(
		CharcterRotation, Rotation,
		GetWorld()->DeltaTimeSeconds,
		CharacterTurnSpeed
	);

	if (CanMove())
		GetMesh()->SetWorldRotation(CharcterRotation);
}

// Find proper cursor position in world space
void APlayerCharacter::UpdateCurosorTransform()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
	{
		FIntPoint point = UGameUserSettings::GetGameUserSettings()->GetScreenResolution();
		FHitResult HitResult;
		bCanAim = GetWorld()->LineTraceSingleByChannel(HitResult,
			GetCameraComponent()->GetComponentLocation(),
			GetCameraComponent()->GetComponentLocation() + CursorLineTraceDistance * GetCameraComponent()->GetForwardVector(),
			ECollisionChannel::ECC_GameTraceChannel1
		);
		FVector MuzzleLocation = WeaponMeshComponent->GetSocketLocation(FName("MuzzleSocket"));
		//if (PlayerController->GetHitResultAtScreenPosition(FVector2D(point.X / 2, point.Y / 2), ECollisionChannel::ECC_Visibility, true, HitResult))
		if (bCanAim)
		{
			CursorLocation = HitResult.Location;
			/*
			FVector CursorFV = HitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(HitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
			bCanAim = true;
			*/
		}
		else
		{
			CursorLocation = GetCameraComponent()->GetComponentLocation() + CursorLineTraceDistance * GetCameraComponent()->GetForwardVector();
			bCanAim = false;
		}
	}
}

// Restore character's speed
// It can be used right after some action that affects character's speed.
void APlayerCharacter::RestorCharacterSpeed()
{
	if (PlayerActionState == EPlayerActionState::EPA_Diving) return;
	if (bRunning)
		TargetSpeed = RunSpeed;
	else
		TargetSpeed = WalkSpeed;

	if (bRMBInput)
		TargetSpeed = ADSSpeed;

	if (bCrouch)
		TargetSpeed *= 0.5f;
}

// Fire weapon and play fire animation of player character
void APlayerCharacter::Fire()
{
	if (!bLMBInput || bMuzzleUp) return;

	if (PlayerActionState != EPlayerActionState::EPA_Idle) return;

	float FireIntervalTime = 60.f / (BaseGunRPM * GunRPMMultiplier);

	if (TimeAfterShot < FireIntervalTime) return;

	if (RoundRemain == 0)
	{
		// Play empty shot sound
		UE_LOG(LogTemp, Warning, TEXT("Remain Round : %d"), RoundRemain);
		UGameplayStatics::PlaySound2D(GetWorld(), EmptyShotSoundCue);
		bTryFire = false;
		return;
	}
	
	TimeAfterShot = 0.f;
	RoundRemain -= 1;
	UE_LOG(LogTemp, Warning, TEXT("Remain Round : %d"), RoundRemain);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(FireMontage, 2.f);
		AnimInstance->Montage_JumpToSection(FName("Fire_Rifle"), FireMontage);

		UGameplayStatics::PlaySound2D(GetWorld(), GunFireSoundCue);

		FVector MuzzleLocation = WeaponMeshComponent->GetSocketLocation(FName("MuzzleSocket"));
		FRotator MuzzleRotation = WeaponMeshComponent->GetSocketRotation(FName("MuzzleSocket"));;
		FVector MuzzleScale = FVector(0.08f);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleParticleSystem, MuzzleLocation,MuzzleRotation,MuzzleScale);

		if (!EquippedWeapon) return;

		// 1. Get Spreaded direction
		FVector OriginalDirection = GetCameraComponent()->GetForwardVector();
		float RandomRadian = FMath::FRandRange(-PI,PI);
		float RandomSin = FMath::Sin(RandomRadian);
		float RandomCos = FMath::Cos(RandomRadian);
		float RandomSpread = FMath::FRandRange(-Spread, Spread);
		FVector RandomSpreadDirection = 
			RandomSin * GetCameraComponent()->GetUpVector() + 
			RandomCos * GetCameraComponent()->GetRightVector();
		FVector SpreadedDirection = OriginalDirection * 100.f + RandomSpread * RandomSpreadDirection;

		// 2. Find cursor offset
		FMinimalViewInfo ViewInfo;
		GetCameraComponent()->GetCameraView(GetWorld()->DeltaTimeSeconds, ViewInfo);
		FMatrix ProjectionMatrix = ViewInfo.CalculateProjectionMatrix();
		FVector4 ProjectedPosition = ProjectionMatrix.TransformPosition(FVector(RandomSpread * RandomCos, -RandomSpread * RandomSin, 100.f));
		ProjectedPosition = ProjectedPosition / ProjectedPosition.W; // Into NDC : [-1,1]x[-1,1]
		FVector2D ScreenSize;
		GEngine->GameViewport->GetViewportSize(ScreenSize);
		CursorOffset = FVector2D(ProjectedPosition.X, ProjectedPosition.Y) * ScreenSize * 0.5;
		
		// 2. Find TargetLocation using linetrace
		FHitResult HitResult;
		bool bTargetHit = GetWorld()->LineTraceSingleByChannel(HitResult,
			GetCameraComponent()->GetComponentLocation(),
			GetCameraComponent()->GetComponentLocation() + CursorLineTraceDistance * SpreadedDirection.GetSafeNormal(),
			ECollisionChannel::ECC_GameTraceChannel1
		);
		FVector TargetLocation;
		if (bTargetHit)
		{
			TargetLocation = HitResult.ImpactPoint;
		}
		else
		{
			TargetLocation = GetCameraComponent()->GetComponentLocation() +
								CursorLineTraceDistance * SpreadedDirection.GetSafeNormal();
		}

		EquippedWeapon->Fire(
			MuzzleLocation,
			TargetLocation - MuzzleLocation
		);

		// Spread Increase
		Spread = FMath::FInterpTo(
			Spread,
			BaseMaxSpread * ((bRMBInput) ? 1.0f:2.0f),
			FireIntervalTime,
			BaseSpreadIncreaseSpeed
		);

		// Give recoil velocity
		RecoilVelocity.Yaw = FMath::FRandRange(-BaseHorizontalRecoil, BaseHorizontalRecoil);
		RecoilVelocity.Pitch = BaseVerticalRecoil;
		RecoilVelocity.Roll = 0.f;
		RecoilDampingTime = 0.f;

		GetWorldTimerManager().ClearTimer(CursorResetTimerHandle);
		GetWorldTimerManager().SetTimer(CursorResetTimerHandle,this,&APlayerCharacter::ResetCursorOffset, FireIntervalTime + 0.3f);
		GetWorldTimerManager().ClearTimer(RecoilResetTimerHandle);
		GetWorldTimerManager().SetTimer(RecoilResetTimerHandle, this, &APlayerCharacter::ResetRecoilVelocity, FireIntervalTime + 0.1f);
	}
	if (bAutomatic)
	{
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &APlayerCharacter::Fire, FireIntervalTime);
	}
	else
	{
		Spread = BaseMaxSpread;
		bTryFire = false;
	}
}

void APlayerCharacter::ThrowGrenade()
{
	if (GrenadeRemain == 0)
	{
		// Say No Grenade
		return;
	}
	if (PlayerActionState != EPlayerActionState::EPA_Idle) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		PlayerActionState = EPlayerActionState::EPA_Throwing;

		AnimInstance->Montage_Play(FireMontage, 1.f);
		AnimInstance->Montage_JumpToSection(FName("Throw_Grenade"), FireMontage);
	}
}

void APlayerCharacter::ResetLMBLook()
{
	if (!bLMBInput)
		bLMBLook = false;
}