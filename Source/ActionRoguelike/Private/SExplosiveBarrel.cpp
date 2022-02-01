// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"
#include "DrawDebugHelpers.h"
#include "SGameInstance.h"

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
	ForceComp->ImpulseStrength = 1000.0f;
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
	ForceComp->FireImpulse();

	FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());

	if (Cast<USGameInstance>(GetGameInstance())->DrawDebugInfo)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Hit the barrel!");
		UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, at game time %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);
		DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);
	}
}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

