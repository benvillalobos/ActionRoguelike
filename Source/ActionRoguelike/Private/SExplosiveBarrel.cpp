// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	RootComponent = MeshComp;
	MeshComp->SetSimulatePhysics(true);

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->Radius = 1000.0f;
	ForceComp->ImpulseStrength = 2500.0f;
	ForceComp->bImpulseVelChange = true;
	ForceComp->SetupAttachment(MeshComp);

}

void ASExplosiveBarrel::PostInitializeComponents()
{
	// Don't forget to call parent function
	Super::PostInitializeComponents();
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::Explode);
}

void ASExplosiveBarrel::Explode(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Hit the barrel!");
	ForceComp->FireImpulse();
}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

