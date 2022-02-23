// Fill out your copyright notice in the Description page of Project Settings.


#include "STargetDummy.h"

// Sets default values
ASTargetDummy::ASTargetDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

		MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
    
    	RootComponent = MeshComp;
    
    	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
    
    	AttributeComp->OnHealthChanged.AddDynamic(this, &ASTargetDummy::OnHealthChanged);

}



// Called when the game starts or when spawned
void ASTargetDummy::BeginPlay()
{
	Super::BeginPlay();
}

void ASTargetDummy::OnHealthChanged(AActor* OtherActor, USAttributeComponent* OwningComp, float newHealth, float Delta)
{
	if (Delta < 0.0f)
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
}

// Called every frame
void ASTargetDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

