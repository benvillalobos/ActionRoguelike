// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameInstance.h"
#include "SInteractionComponent.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()
	
protected:
	// Creating a generic projectile variable. (Q: Why not just make it an AActor?)
	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> BlackHoleProjectileClass;

	UPROPERTY(EditAnywhere, Category="Attack")
	UAnimMontage* AttackAnim;
	
	FTimerHandle TimerHandle_PrimaryAttack;

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	void MoveForward(float val);
	void MoveRight(float val);
	void PrimaryAttack();
	void BlackHoleAttack();
	void PrimaryAttack_TimeElapsed();
	void BlackHoleAttack_TimeElapsed();
	void PrimaryInteract();
	void LaunchStandardProjectile(TSubclassOf<AActor> projectileClass);
	USGameInstance* gameInstance;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Trigger global var to enable/disable drawing debug symbols (arrows, line traces, etc.)
	void DebugButton();


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
