// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bIsOpening = true;
}

// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;
	
	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	bIsOpening = false;
	TargetPitch = 110.0f;
	currentPitch = 0.0f;
}

// Called when the game starts or when spawned
void ASItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsOpening && currentPitch <= TargetPitch)
	{
		currentPitch++;
		// Relative to what it's attached to: the base mesh
		LidMesh->SetRelativeRotation(FRotator(currentPitch, 0.0f, 0));

		if (currentPitch >= TargetPitch)
		{
			bIsOpening = false;
		}
	}

}
