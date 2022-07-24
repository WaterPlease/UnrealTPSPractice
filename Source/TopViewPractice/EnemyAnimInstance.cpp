// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "EnemyCharacter.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	if (Pawn && Enemy) return;
	
	Pawn = TryGetPawnOwner();
	if (!Pawn) return;

	Enemy = Cast<AEnemyCharacter>(Pawn);
}

void UEnemyAnimInstance::UpdateProperties()
{
	if (!Enemy)
	{
		NativeInitializeAnimation();
	}

	if (Enemy)
	{
		Speed = Enemy->GetVelocity().Size();
	}
}