// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "AIController.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharacterController.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttackSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphere"));
	AttackSphere->SetupAttachment(GetRootComponent());

	LookAtSphere = CreateDefaultSubobject<USphereComponent>(TEXT("LookAtSphere"));
	LookAtSphere->SetupAttachment(GetRootComponent());

	AttackCollisionA = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollisionA"));
	AttackCollisionA->AttachTo(GetMesh(), FName("AttackSocketA"));
	AttackCollisionB = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollisionB"));
	AttackCollisionB->AttachTo(GetMesh(), FName("AttackSocketB"));

	AttackCollisions.Add(AttackCollisionA);
	AttackCollisions.Add(AttackCollisionB);

	// Radar point load
	RadarPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RadarPoint"));
	RadarPoint->SetupAttachment(GetRootComponent());

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(
		TEXT("StaticMesh'/Game/HUD/Mesh/Plane.Plane'")
	);
	RadarPoint->SetStaticMesh(PlaneMesh.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> RadarPointMaterial(
		TEXT("MaterialInstanceConstant'/Game/HUD/Materials/MinimapPoint_Enemy_MAT.MinimapPoint_Enemy_MAT'")
	);
	RadarPoint->SetMaterial(0, RadarPointMaterial.Object);
	RadarPoint->SetRelativeLocation(FVector(0.0f, 0.0f, 1000.f));

	Score = 100.f;
	MaxHealth = 100.f;
	Health = MaxHealth;
	Damage = 10.f;

	EnemyActionState = EEnemyActionState::EEA_Idle;
	bCanAttack = false;
	bWaitAttack = false;

	bTryLookAt = false;

	// Prevent rotation of character by camera
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	AttackDelay = 1.5f;

	NumAttackType = 1;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AAIController>(GetController());

	// LookAtSphere Configuration
	LookAtSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::LookAtSphereBeginOverlap);
	LookAtSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::LookAtSphereEndOverlap);
	LookAtSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LookAtSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LookAtSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	// AttackSphere Configuration
	AttackSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::AttackSphereBeginOverlap);
	AttackSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::AttackSphereEndOverlap);
	AttackSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttackSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AttackSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	// AttackCollision Configuration
	AttackCollisionA->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::AttackCollisionBeginOverlap);
	AttackCollisionA->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::AttackCollisionEndOverlap);
	AttackCollisionA->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackCollisionA->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AttackCollisionA->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	AttackCollisionA->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Overlap);
	AttackCollisionA->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	
	AttackCollisionB->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::AttackCollisionBeginOverlap);
	AttackCollisionB->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::AttackCollisionEndOverlap);
	AttackCollisionB->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackCollisionB->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AttackCollisionB->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	AttackCollisionB->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Overlap);
	AttackCollisionB->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);


	// Mesh Collision tracing channel Configuration
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	// Make capsule component ignore ECC_Camera collision channel
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);

	RadarPoint->SetRelativeScale3D(FVector((2.f / 34.f) * GetCapsuleComponent()->GetUnscaledCapsuleRadius()));
	RadarPoint->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// State transition
	switch (EnemyActionState)
	{
	case EEnemyActionState::EEA_Idle:
		if (bCanAttack)
			Attack();
		else
			ChasePlayer();
		break;
	case EEnemyActionState::EEA_Chase:
		if (bCanAttack)
			Attack();
		else
		{
			if (AIController &&
				AIController->GetMoveStatus()
				== EPathFollowingStatus::Idle)
			{
				ChasePlayer();
			}
		}
	case EEnemyActionState::EEA_Attack:
		break;
	case EEnemyActionState::EEA_Die:
		break;
	default:
		break;
	}

	if (bTryLookAt &&
		EnemyActionState != EEnemyActionState::EEA_Attack)
		LookAtPlayer(DeltaTime);
}

void AEnemyCharacter::AttackSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((AActor*)PlayerCharacter != OtherActor) return;

	bCanAttack = true;
}

void AEnemyCharacter::AttackSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((AActor*)PlayerCharacter != OtherActor) return;

	bCanAttack = false;
}

void AEnemyCharacter::AttackCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AIController)
		AIController->StopMovement();
	// overlap with player
	if (Cast<APawn>(OtherActor))
	{
		if(!Cast<APlayerCharacter>(OtherActor)) return;

		// Apply damage to player
		if (GetPlayerCharacter())
		{
			UGameplayStatics::ApplyDamage(
				PlayerCharacter,
				Damage,
				GetController(),
				this,
				DamageType
			);
		}
		return;
	}
	
	UBoxComponent* AttackCollision = Cast<UBoxComponent>(OverlappedComponent);
	
	if (!AttackCollision) return;
	// overlap with destructible
	FVector Impulse = AttackCollision->GetComponentVelocity() * 50.f;
	OtherComp->AddImpulseAtLocation(Impulse, SweepResult.Location);
	// Apply damage to object
	//...
}

void AEnemyCharacter::AttackCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Nothing to Do...
}

void AEnemyCharacter::LookAtSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);

	if (!Player) return;

	bTryLookAt = true;
}

void AEnemyCharacter::LookAtSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);

	if (!Player) return;

	bTryLookAt = false;
}

void AEnemyCharacter::ActivateAttackCollision(int idx)
{
	AttackCollisions[idx]->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemyCharacter::DeactivateAttackCollision(int idx)
{
	AttackCollisions[idx]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemyCharacter::AttackDone()
{
	EnemyActionState = EEnemyActionState::EEA_Idle;
	UE_LOG(LogTemp, Warning, TEXT("AttackDone"));
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AEnemyCharacter::WaitAttackDone, AttackDelay);
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (DamageHistory.Contains(DamageCauser))
		return DamageAmount;

	DamageHistory.Add(DamageCauser);
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(EventInstigator);
	if (EventInstigator)
	{
		if (GetPlayerCharacter() && EnemyActionState != EEnemyActionState::EEA_Die)
		{
			PlayerCharacter->ShowHitmarker();
		}
	}

	Health -= DamageAmount;
	if (Health < 0.f || FMath::IsNearlyZero(Health))
	{
		Die();
	}
	return DamageAmount;
}

APlayerCharacter* AEnemyCharacter::GetPlayerCharacter()
{
	if (PlayerCharacter) return PlayerCharacter;

	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	return PlayerCharacter;
}

void AEnemyCharacter::LookAtPlayer(float DeltaTime)
{
	if (!PlayerCharacter) return;

	FRotator EnemyRotation = GetActorRotation();
	FVector EnemyLocation = GetActorLocation();
	FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	FRotator TargetRotation = (PlayerLocation - EnemyLocation).Rotation();
	TargetRotation.Pitch = 0.f;
	TargetRotation.Roll = 0.f;

	EnemyRotation = FMath::RInterpTo(
		EnemyRotation,
		TargetRotation,
		DeltaTime,
		5.f
	);

	SetActorRotation(EnemyRotation);
}

void AEnemyCharacter::ChasePlayer()
{
	if (!AIController ||        // cannot find ai controller
	    !GetPlayerCharacter())  // cannot find player's character
		return;

	// PlayerChaseRequest configuration
	FAIMoveRequest PlayerChaseRequest;
	PlayerChaseRequest.SetGoalActor((AActor*)GetPlayerCharacter());
	PlayerChaseRequest.SetAcceptanceRadius(GetCapsuleComponent()->GetScaledCapsuleRadius() * 0.5f);
	FNavPathSharedPtr NavPathPtr;

	AIController->MoveTo(PlayerChaseRequest, &NavPathPtr);

	// Handle NavPathPtr...


	// Change State
	EnemyActionState = EEnemyActionState::EEA_Chase;
}

void AEnemyCharacter::Attack()
{
	if (bWaitAttack) return;
	bWaitAttack = true;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		int AttackType = FMath::Rand() % NumAttackType;

		AnimInstance->Montage_Play(CombatMontage);
		AnimInstance->Montage_JumpToSection(FName(TEXT("Attack") + FString::FromInt(AttackType)), CombatMontage);

		// Change State
		EnemyActionState = EEnemyActionState::EEA_Attack;

		AIController->StopMovement();
	}
}

void AEnemyCharacter::Die()
{
	EnemyActionState = EEnemyActionState::EEA_Die;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_EngineTraceChannel1, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);

	AIController->StopMovement();

	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController)
		PlayerController->EnemyKill(this);

	GetWorldTimerManager().SetTimer(DeadTimer, this, &AEnemyCharacter::DieDone, 5.f);
}

void AEnemyCharacter::DieDone()
{
	Destroy();
}