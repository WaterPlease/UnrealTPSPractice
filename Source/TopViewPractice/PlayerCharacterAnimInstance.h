// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnimInstance.generated.h"

enum class EPlayerActionState : uint8;

/**
 * 
 */
UCLASS()
class TOPVIEWPRACTICE_API UPlayerCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class APlayerCharacter* PlayerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float HorizontalSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float VerticalSpeed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bMuzzleUp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EPlayerActionState PlayerActionState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector CapsuleLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float TimeMuzzleMoving;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float TimeToMoveMuzzle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ADSSpinePitch;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bADS;
	
	
public:
	virtual void NativeInitializeAnimation() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void UpdateProperties();

	bool bIKRotation;
	float IKRotaionStrength;

	float IKTraceDistance;
	float IKScale;
	float IKInterpSpeed;
	float IKUnderGroundDepth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK")
	float IKTraceRightFoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK")
	float IKTraceLeftFoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK")
	FRotator IKRotationRightFoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK")
	FRotator IKRotationLeftFoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK")
	float IKTraceHips;

	
	void UpdateIKTraceDistance();
	
	float IKFootTrace(FName SocketName, float TraceDistance, FRotator& OutHitRotation);
};
