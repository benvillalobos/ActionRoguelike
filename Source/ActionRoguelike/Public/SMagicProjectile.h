// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBaseProjectile.h"
#include "SMagicProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public ASBaseProjectile
{
	GENERATED_BODY()
	
public:
	// UFUNCTION allows OnComponentOverlap to bind properly
	UFUNCTION()
	void OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult&  SweepResult);
	// Sets default values for this actor's properties
	ASMagicProjectile();
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
