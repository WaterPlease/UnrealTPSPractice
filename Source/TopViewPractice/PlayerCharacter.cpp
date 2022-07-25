// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
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


	/////////////////////////////////////////////////////////////////////////////
	// From : Unreal template(TopDown) code.
	// 
	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	/////////////////////////////////////////////////////////////////////////////

	// Weapon Mesh
	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
	//WeaponMeshComponent->AttachTo(GetMesh(), FName("LeftHandSocket"));
	WeaponMeshComponent->AttachTo(GetMesh(), FName("RightHandSocket"));
	//WeaponMeshComponent->AttachTo(GetRootComponent());

	// Muzzle Up Triggerbox
	MuzzleUpTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MuzzleUpTriggerBox"));
	MuzzleUpTriggerBox->SetupAttachment(GetMesh());

	// Input State Initialization
	bLMBInput = false;
	bRMBInput = false;
	bInteractionInput = false;
	bDodgeInput = false;
	bMuzzleUp = false;
	bRunning = false;

	PlayerActionState = EPlayerActionState::EPA_Idle;

	// User Field Initialization
	CameraDistance = 800.f;
	CameraMaxDistance = 800.f;
	CameraMinDistance = 100.f;
	CameraRotationAngle = -60.f;
	CameraMaxRotationAngle = -30.f;
	CameraMinRotationAngle = -60.f;
	CameraSensitivity = 30.f;
	CameraYaw = 0.f;
	ZoomLevel = 3;
	SetCameraDistance(CameraDistance);
	SetCameraRotationAngle(CameraRotationAngle);

	BaseGunRPM = 650.f;
	GunRPMMultiplier = 1.f;
	RoundCapacity = 30;
	RoundRemain = 31;
	MaxHealth = 100.f;
	Health = 100.f;
	HealthRegenRate = 10.f;
	HealthRegenDelay = 10.f;
	WalkSpeed = 200.f;
	RunSpeed = 600.f;
	DiveAcceleration = 3.f;
	DiveDuration = 1.f;
	CharacterTurnSpeed = 15.f;
	CharacterAimTurnSpeed = 15.f;

	bIKRotation = true;
	IKRotaionStrength = 1.2f;

	TimeMuzzleMoving = 0.f;
	TimeToMoveMuzzle = 1.0f;
	
	SpinePitch = 0.f;

	CharcterRotation = FRotator(0.f);

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
	SetCameraRotationAngle(CameraRotationAngle);

	// Muzzle up trigger event
	MuzzleUpTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnMuzzleTriggerOverlapBegin);
	MuzzleUpTriggerBox->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnMuzzleTriggerOverlapEnd);
	MuzzleUpTriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MuzzleUpTriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	MuzzleUpTriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
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
	FVector NeckToCursor = GetMesh()->GetSocketLocation("Neck") - CursorToWorld->GetComponentLocation();
	float DistOnXY = FMath::Sqrt(NeckToCursor.X * NeckToCursor.X + NeckToCursor.Y * NeckToCursor.Y);
	SpinePitch = FMath::RadiansToDegrees(FMath::Atan2(NeckToCursor.Z, DistOnXY));


	// Smooth camera
	float TempDist = CameraBoom->TargetArmLength;
	float TempRot = CameraBoom->GetRelativeRotation().Pitch;
	if (!FMath::IsNearlyEqual(TempDist, CameraDistance) ||
		!FMath::IsNearlyEqual(TempRot, CameraRotationAngle))
	{
		TempDist = FMath::FInterpTo(TempDist, CameraDistance, DeltaTime, CameraBoom->CameraLagSpeed);
		TempRot = FMath::FInterpTo(TempRot, CameraRotationAngle, DeltaTime, CameraBoom->CameraLagSpeed);

		CameraBoom->TargetArmLength = TempDist;
		CameraBoom->SetRelativeRotation(FRotator(TempRot,0.f,0.f));
		CameraBoom->AddWorldRotation(FRotator(0.f,CameraYaw, 0.f));
	}
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
}

// Vertical movement input handler
void APlayerCharacter::InputVertical(float Value)
{
	if (CanMove())
	{
		FVector UpVector = GetCameraComponent()->GetUpVector();
		UpVector.Z = 0.f;
		FVector Direction = UpVector.GetSafeNormal();
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
	if (!FMath::IsNearlyZero(Value))
	{
		float DeltaDistance = (CameraMaxDistance - CameraMinDistance) / (float)ZoomLevel;
		float DeltaRotation = (CameraMaxRotationAngle - CameraMinRotationAngle) / (float)ZoomLevel;

		CameraDistance = FMath::Clamp<float>(CameraDistance - DeltaDistance * Value, CameraMinDistance, CameraMaxDistance);
		CameraRotationAngle = FMath::Clamp<float>(CameraRotationAngle + DeltaRotation * Value, CameraMinRotationAngle, CameraMaxRotationAngle);
	}
}

// Horizontal turn (mouse drag) input handler
void APlayerCharacter::HorizontalTurn(float Value)
{
	if (bCameraRotationInput && !FMath::IsNearlyZero(Value))
	{
		CameraBoom->AddWorldRotation(FRotator(0.f, CameraSensitivity * Value, 0.f));

		CameraYaw = CameraBoom->GetComponentRotation().Yaw;
	}
}

void APlayerCharacter::LMBDown()
{
	bLMBInput = true;

	Fire();
}

void APlayerCharacter::LMBUp()
{
	bLMBInput = false;
}

void APlayerCharacter::RMBDown()
{
	bRMBInput = true;
}

void APlayerCharacter::RMBUp()
{
	bRMBInput = false;
}

void APlayerCharacter::RunDown()
{
	bRunInput = true;

	bRunning = !bRunning;
	RestorCharacterSpeed();
}

void APlayerCharacter::RunUp()
{
	bRunInput = false;
}

void APlayerCharacter::GrenadeDown()
{
	bGrenadeInput = true;

	if (PlayerActionState != EPlayerActionState::EPA_Idle) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		PlayerActionState = EPlayerActionState::EPA_Throwing;

		AnimInstance->Montage_Play(FireMontage, 1.f);
		AnimInstance->Montage_JumpToSection(FName("Throw_Grenade"), FireMontage);
	}
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

		//WeaponMeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		//WeaponMeshComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, FName("RightHandSocket"));

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

	if (PlayerActionState == EPlayerActionState::EPA_Diving) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Stop(0.1f, FireMontage);
		AnimInstance->Montage_Play(FireMontage, 2.37f / DiveDuration);
		AnimInstance->Montage_JumpToSection(FName("Dive"), FireMontage);


		// Find direction of diving
		// => Moving direction or character's looking direction if no movement
		FVector UpVector = GetCameraComponent()->GetUpVector();
		UpVector.Z = 0.f;
		UpVector = UpVector.GetSafeNormal();
		FVector RightVector = GetCameraComponent()->GetRightVector();
		RightVector.Z = 0.f;
		RightVector = RightVector.GetSafeNormal();

		DiveDirection = UpVector * GetInputAxisValue(FName("Vertical")) +
							 RightVector * GetInputAxisValue(FName("Horizontal"));
		if (DiveDirection.IsNearlyZero())
			DiveDirection = GetMesh()->GetRightVector();
		else
			DiveDirection = DiveDirection.GetSafeNormal();

		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(FVector(0.f), DiveDirection);
		Rotation.Pitch = 0.f;
		Rotation.Roll = 0.f;
		Rotation.Yaw -= 90.f;
		GetMesh()->SetWorldRotation(Rotation);
		DiveRotation = Rotation;

		// Set moving speed
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed * DiveAcceleration;

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
		PlayerActionState != EPlayerActionState::EPA_Diving;
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

	BaseGunRPM = Weapon->BaseGunRPM;
	RoundCapacity = Weapon->RoundCapacity;
	RoundRemain = RoundCapacity+1;

	if (OldWeapon)
	{
		OldWeapon->Destroy();
	}
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

float APlayerCharacter::GetCameraDistance()
{ 
	return CameraDistance;
}

void APlayerCharacter::SetCameraDistance(float Value)
{
	CameraDistance = Value;
}

float APlayerCharacter::GetCameraRotationAngle()
{
	return CameraRotationAngle; 
}

void APlayerCharacter::SetCameraRotationAngle(float Value)
{
	CameraRotationAngle = Value;
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
		FVector CursorLocation = CursorToWorld->GetComponentLocation();

		Rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CursorLocation);

		Rotation.Pitch = 0.f;
		Rotation.Roll = 0.f;
		Rotation.Yaw -= 90.f;
	}

	CharcterRotation = FMath::RInterpTo(
		CharcterRotation, Rotation,
		GetWorld()->DeltaTimeSeconds,
		CharacterTurnSpeed
	);

	GetMesh()->SetWorldRotation(CharcterRotation);
}

// Find proper cursor position in world space
void APlayerCharacter::UpdateCurosorTransform()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
	{
		FHitResult HitResult;
		if (MultiHitUnderCursor(HitResult))
		{
			FVector CursorFV = HitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(HitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}
}

// Fire weapon and play fire animation of player character
void APlayerCharacter::Fire()
{
	if (!bLMBInput || bMuzzleUp) return;

	if (PlayerActionState != EPlayerActionState::EPA_Idle) return;

	if (RoundRemain == 0)
	{
		// Play empty shot sound
		// ...
		UE_LOG(LogTemp, Warning, TEXT("Remain Round : %d"), RoundRemain);
		UGameplayStatics::PlaySound2D(GetWorld(), EmptyShotSoundCue);
		bLMBInput = false;
		return;
	}

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


		if (EquippedWeapon)
		{
			FVector TargetLocation = CursorToWorld->GetComponentLocation();

			if (!bRMBInput)
			{
				TargetLocation.Z = MuzzleLocation.Z;
			}

			EquippedWeapon->Fire(
				MuzzleLocation,
				TargetLocation - MuzzleLocation
			);
		}
	}
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &APlayerCharacter::Fire, 60.f / (BaseGunRPM * GunRPMMultiplier));
}

// Restore character's speed
// It can be used right after some action that affects character's speed.
void APlayerCharacter::RestorCharacterSpeed()
{
	if (PlayerActionState == EPlayerActionState::EPA_Diving) return;
	if (bRunning)
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	else
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}


// Find "proper" cursor location in world space
// proper location means location on ground or other location on objects
// except the wall between character and camera
bool APlayerCharacter::MultiHitUnderCursor(FHitResult& CursorHitResult)
{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->Player);
		bool bHit = false;
		if (LocalPlayer && LocalPlayer->ViewportClient)
		{
			FVector2D MousePosition;
			if (!LocalPlayer->ViewportClient->GetMousePosition(MousePosition))
				return false;
			//bHit = PlayerController->GetHitResultAtScreenPosition(MousePosition, TraceChannel, bTraceComplex, HitResult);
			FVector WorldOrigin;
			FVector WorldDirection;
			if (!PlayerController->GetHUD() != NULL && PlayerController->GetHUD()->GetHitBoxAtCoordinates(MousePosition, true)) return false;
			if (!UGameplayStatics::DeprojectScreenToWorld(PlayerController, MousePosition, WorldOrigin, WorldDirection)) return false;

			TArray<FHitResult> HitResults;

			bHit = GetWorld()->LineTraceMultiByChannel(HitResults,
				WorldOrigin, WorldOrigin + WorldDirection * PlayerController->HitResultTraceDistance,
				ECollisionChannel::ECC_GameTraceChannel1);
			for (FHitResult HitResult : HitResults)
			{
				if (HitResult.bBlockingHit)
				{
					bHit = true;
					CursorHitResult = HitResult;
					break;
				}

				FVector HitLocation = HitResult.Location;
				FVector XYDelta = HitLocation - GetActorLocation();
				XYDelta.Z = 0.f;

				if (FVector::DotProduct(XYDelta, HitResult.Normal) > 0.f) continue;

				bHit = true;
				CursorHitResult = HitResult;
				break;
			}
		}

		if (!bHit)	//If there was no hit we reset the results. This is redundant but helps Blueprint users
		{
			CursorHitResult = FHitResult();
		}

		return bHit;
}
