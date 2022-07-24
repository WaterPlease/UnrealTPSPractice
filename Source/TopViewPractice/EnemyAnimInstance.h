// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TOPVIEWPRACTICE_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly);
	class APawn* Pawn;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly);
	class AEnemyCharacter* Enemy;

	//////////////////////////////////////////////
	// Animation Properties
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly);
	float Speed;





	//////////////////////////////////////////////

	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = "AnimationProperties")
	void UpdateProperties();
};
