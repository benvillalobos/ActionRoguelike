// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "SAttributeComponent.h"
#include "SGameInstance.h"
#include "Particles/ParticleSystemComponent.h"

void ASMagicProjectile::OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		// StaticClass: https://youtu.be/IaU2Hue-ApI?t=262
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));

		if (AttributeComp)
		{
			AttributeComp->ModifyHealth(Damage);
			if (Cast<USGameInstance>(GetGameInstance())->DrawDebugInfo)
			{
				UE_LOG(LogTemp, Error, TEXT("OtherActor: %s, was hit at game time %f for %f damage"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds, -20.0f);
			}
			Destroy();
		}
	}
}

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnComponentOverlap);
}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}


// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

