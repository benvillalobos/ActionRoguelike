// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"

#include "DrawDebugHelpers.h"
#include "SCharacter.h"
#include "SGameplayInterface.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void USInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();
	
	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	float distance = 300.0f;

	// Draw a line to see where we were looking when we interacted
	FVector End = EyeLocation + (EyeRotation.Vector() * distance);
	FHitResult LineHit;
	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(LineHit, EyeLocation, End, ObjectQueryParams);
	DrawDebugLine(GetWorld(), EyeLocation, End, bBlockingHit ? FColor::Green : FColor::Red, false, 2.0f, 0, 2.0f);

	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	float Radius = 50.0f;
	Shape.SetSphere(Radius);

	// Make sure the hit detection is "wide"
	bool bSweepHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (gameInstance && gameInstance->DrawDebugInfo)
			{
				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, bSweepHit ? FColor::Green : FColor::Red, false, 2.0f, 0, 2.0f);
			}
			
			if (HitActor->Implements<USGameplayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);
				ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
				break; //stop once we've interacted with something
			}
		}
	}

}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	gameInstance = Cast<USGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

