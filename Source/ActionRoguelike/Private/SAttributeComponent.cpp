// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	
}


void USAttributeComponent::ModifyHealth(float val)
{
	Health += val;
	OnHealthChanged.Broadcast(nullptr, this, Health, val);
}

// Called when the game starts
void USAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void USAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

