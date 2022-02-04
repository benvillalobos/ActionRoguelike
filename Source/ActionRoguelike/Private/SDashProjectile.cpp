// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"
#include "AITypes.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ASDashProjectile::ASDashProjectile() : Super()
{
	MovementComp->InitialSpeed = 6000.0f;
}

void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);

	GetWorldTimerManager().SetTimer(TimerHandle_Explosion,this, &ASDashProjectile::Explosion, 0.2f);
}

void ASDashProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != GetInstigator())
		Explosion();
}

void ASDashProjectile::Explosion()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Explosion);

	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEffect, GetActorLocation(), GetActorRotation());
	EffectComp->DeactivateSystem();
	
	GetWorldTimerManager().SetTimer(TimerHandle_Teleport, this, &ASDashProjectile::Teleport, 0.2f);
}

void ASDashProjectile::Teleport()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Teleport);
	
	APawn* instigator = GetInstigator();
	instigator->TeleportTo(this->GetActorLocation(), instigator->GetActorRotation(), false, true);
	this->Destroy();
}

void ASDashProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
