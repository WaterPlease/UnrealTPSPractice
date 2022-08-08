// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionBullet.h"
#include "Particles/ParticleSystemComponent.h"

AMinionBullet::AMinionBullet()
{
	BulletParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	BulletParticleSystemComponent->SetupAttachment(GetRootComponent());

	RemainBounce = 0;
}