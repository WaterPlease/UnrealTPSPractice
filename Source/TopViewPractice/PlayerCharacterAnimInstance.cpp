// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"


enum class EPlayerActionState : uint8;

void UPlayerCharacterAnimInstance::NativeInitializeAnimation()
{
	PlayerCharacter = nullptr;

	HorizontalSpeed = 0.f;
	VerticalSpeed = 0.f;
	bMuzzleUp = false;

	PlayerActionState = EPlayerActionState::EPA_Idle;

	IKInterpSpeed = 5.f;
	IKTraceLeftFoot = 0.f;
	IKTraceRightFoot = 0.f;
	IKRotationLeftFoot = FRotator(0.f);
	IKRotationRightFoot = FRotator(0.f);
	IKHandGripLocation = FVector(0.f);
	TimeMuzzleMoving = 0.f;
	TimeToMoveMuzzle = 1.f;

	ADSSpinePitch = 0.f;
	bADS = false;
	bTryFire = false;
	bRunning = false;
	bCrouch = false;
	bFalling = false;

	CapsuleLocation = FVector(0.f);
}

void UPlayerCharacterAnimInstance::UpdateProperties()
{
	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
	if (PlayerCharacter)
	{
		FVector ForwardVector = PlayerCharacter->GetMesh()->GetForwardVector();
		FVector RightVector = PlayerCharacter->GetMesh()->GetRightVector();
		FVector VelocityVector = PlayerCharacter->GetVelocity();
		VerticalSpeed = FVector::DotProduct(RightVector, VelocityVector);
		HorizontalSpeed = FVector::DotProduct(-ForwardVector, VelocityVector);

		bMuzzleUp = PlayerCharacter->bMuzzleUp;
		TimeMuzzleMoving = PlayerCharacter->TimeMuzzleMoving;
		TimeToMoveMuzzle = PlayerCharacter->TimeToMoveMuzzle;
		
		bIKRotation = PlayerCharacter->bIKRotation;
		IKRotaionStrength = PlayerCharacter->IKRotaionStrength;

		PlayerActionState = PlayerCharacter->PlayerActionState;

		bADS = PlayerCharacter->bRMBInput;
		bTryFire = PlayerCharacter->bTryFire;
		bRunning = PlayerCharacter->bRunning;
		bCrouch = PlayerCharacter->bCrouch;
		bFalling = PlayerCharacter->GetCharacterMovement()->IsFalling();
		
		
		ADSSpinePitch = FMath::FInterpTo(
			ADSSpinePitch,
			PlayerCharacter->SpinePitch,
			GetWorld()->DeltaTimeSeconds,
			PlayerCharacter->CharacterAimTurnSpeed
		);
		/*
		if (bADS || bTryFire)
		{
			
		}
		else
		{
			ADSSpinePitch = FMath::FInterpTo(
				ADSSpinePitch,
				0.f,
				GetWorld()->DeltaTimeSeconds,
				PlayerCharacter->CharacterAimTurnSpeed
				);
		}
		*/

		CapsuleLocation = PlayerCharacter->GetCapsuleComponent()->GetComponentLocation();
		
		UpdateIKTraceDistance();

		float Dt = GetWorld()->DeltaTimeSeconds;

		FRotator TempIKRotation;
		float TempIKTrace = IKFootTrace("RightFootSocket", IKTraceDistance, TempIKRotation);
		IKTraceRightFoot = FMath::FInterpTo(
			IKTraceRightFoot,
			TempIKTrace,
			Dt,
			IKInterpSpeed
		);
		IKRotationRightFoot = FMath::RInterpTo(
			IKRotationRightFoot,
			TempIKRotation,
			Dt,
			IKInterpSpeed
		);
		TempIKTrace = IKFootTrace("LeftFootSocket", IKTraceDistance, TempIKRotation);
		IKTraceLeftFoot = FMath::FInterpTo(
			IKTraceLeftFoot,
			TempIKTrace,
			Dt,
			IKInterpSpeed
		);
		IKRotationLeftFoot = FMath::RInterpTo(
			IKRotationLeftFoot,
			TempIKRotation,
			Dt,
			IKInterpSpeed
		);
		IKTraceHips = FMath::Min<float>(IKTraceRightFoot, IKTraceLeftFoot);
		//IKTraceHips = FMath::Max<float>(IKTraceHips, -50.f);


		IKHandGripLocation = PlayerCharacter->WeaponMeshComponent->GetSocketLocation("GripSocket");
	}
}

void UPlayerCharacterAnimInstance::UpdateIKTraceDistance()
{
	IKScale = PlayerCharacter->GetActorScale().Z;
	float CapsuleHalfHeight = PlayerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	IKUnderGroundDepth = CapsuleHalfHeight * .5f;
	IKTraceDistance = CapsuleHalfHeight + IKUnderGroundDepth;
}

float UPlayerCharacterAnimInstance::IKFootTrace(FName SocketName, float TraceDistance, FRotator& OutHitRotation)
{
	auto Mesh = PlayerCharacter->GetMesh();

	FVector SocketLocation = Mesh->GetSocketLocation(SocketName);
	FVector ActorLocation = PlayerCharacter->GetActorLocation();

	FVector TraceStart(SocketLocation.X, SocketLocation.Y, ActorLocation.Z);
	FVector TraceEnd(SocketLocation.X, SocketLocation.Y, ActorLocation.Z - TraceDistance);

	FHitResult HitResult;
	auto CollisionQueryParam = FCollisionQueryParams::DefaultQueryParam;
	CollisionQueryParam.bDebugQuery = true;
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParam);

	// Draw Debug Line
	/*DrawDebugLine(
		GetWorld(),
		TraceStart,
		HitResult.Location,
		FColor::Red
	);

	DrawDebugSphere(
		GetWorld(),
		HitResult.Location,
		30.f,
		16,
		FColor::Green
	);
	*/

	// float HitDistance = FVector::Distance(HitResult.Location, TraceEnd);
	float FootHeightDelta = HitResult.Location.Z - TraceEnd.Z - IKUnderGroundDepth;
	if (!bHit)
		FootHeightDelta = 0.f;

	// Get foot rotation
	const FVector& Normal = HitResult.Normal;
	
	OutHitRotation.Roll = FMath::RadiansToDegrees(FMath::Atan2(Normal.Y, Normal.Z));
	OutHitRotation.Pitch = -FMath::RadiansToDegrees(FMath::Atan2(Normal.X, Normal.Z));
	OutHitRotation.Yaw = 0.f;
	//OutHitRotation *= IKRotaionStrength;
	if (!bHit || !bIKRotation)
		OutHitRotation = FRotator(0.f);
	//if (FootHeightDelta > 0) FootHeightDelta += 10.f;
	return FootHeightDelta;
}
