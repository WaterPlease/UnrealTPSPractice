// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "AIController.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttackSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphere"));
	AttackSphere->SetupAttachment(GetRootComponent());
	
	AttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollision"));
	AttackCollision->AttachTo(GetMesh(), FName("AttackSocket"));

	EnemyActionState = EEnemyActionState::EEA_Idle;
	bCanAttack = false;
	bWaitAttack = false;

	// Prevent rotation of character by camera
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	AttackDelay = 1.5f;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AAIController>(GetController());

	// AttackSphere Configuration
	AttackSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::AttackSphereBeginOverlap);
	AttackSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::AttackSphereEndOverlap);
	AttackSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttackSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AttackSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	// AttackCollision Configuration
	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::AttackCollisionBeginOverlap);
	AttackCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::AttackCollisionEndOverlap);
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	AttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Overlap);
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
	default:
		break;
	}
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
	// overlap with player
	if (Cast<APawn>(OtherActor))
	{
		if(!Cast<APlayerCharacter>(OtherActor)) return;

		// Apply damage to player
		// ...
		return;
	}
		
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

void AEnemyCharacter::ActivateAttackCollision()
{
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemyCharacter::DeactivateAttackCollision()
{
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemyCharacter::AttackDone()
{
	EnemyActionState = EEnemyActionState::EEA_Idle;

	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AEnemyCharacter::WaitAttackDone, AttackDelay);
}

APlayerCharacter* AEnemyCharacter::GetPlayerCharacter()
{
	if (PlayerCharacter) return PlayerCharacter;

	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	return PlayerCharacter;
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
		AnimInstance->Montage_Play(CombatMontage);
		AnimInstance->Montage_JumpToSection("Attack", CombatMontage);

		// Change State
		EnemyActionState = EEnemyActionState::EEA_Attack;

		AIController->StopMovement();
	}
}