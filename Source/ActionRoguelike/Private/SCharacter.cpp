// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/SCharacter.h"

#include "DrawDebugHelpers.h"
#include "SGameInstance.h"
#include "SMagicProjectile.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Containers/Array.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	gameInstance = Cast<USGameInstance>(GetGameInstance());
	gameInstance->DrawDebugInfo = true;
}

void ASCharacter::MoveForward(float val)
{
	// My original idea
	//AddMovementInput(CameraComp->GetForwardVector(), val, false);

	// Tom Looman's method
	FRotator controlRot = GetControlRotation();
	controlRot.Pitch = 0.0f;
	controlRot.Roll = 0.0f;

	AddMovementInput(controlRot.Vector(), val);
}

void ASCharacter::MoveRight(float val)
{
	// My original idea
	//AddMovementInput(CameraComp->GetRightVector(), val, false);

	// Tom Looman's idea
	FRotator controlRot = GetControlRotation();
	controlRot.Pitch = 0.0f;
	controlRot.Roll = 0.0f;

	// x: forward (red)
	// y: right (green)
	// z: up (blue)

	AddMovementInput(UKismetMathLibrary::GetRightVector(controlRot), val);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	LaunchStandardProjectile(ProjectileClass);
}

void ASCharacter::BlackHoleAttack_TimeElapsed()
{
	LaunchStandardProjectile(BlackHoleProjectileClass);
}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack,this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.18f);
}

void ASCharacter::BlackHoleAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack,this, &ASCharacter::BlackHoleAttack_TimeElapsed, 0.18f);
}

void ASCharacter::LaunchStandardProjectile(TSubclassOf<AActor> projectileClass)
{
	TArray<FHitResult> lineTraceHits;

	FVector cameraLocation = CameraComp->GetComponentLocation();

	// Grab the character's hand right just before the attack
	FTransform spawnTM = FTransform(GetControlRotation(), GetMesh()->GetSocketLocation("Muzzle_01"));
	// Pick a location VERY far away from what the camera is looking at
	FVector lineTraceEnd = cameraLocation + (CameraComp->GetComponentRotation().Vector() * 100000000.0f);
	// Check if shooting a ray from the camera out into the world hits anything
	bool bDidHitSomething = GetWorld()->LineTraceMultiByChannel(lineTraceHits, cameraLocation, lineTraceEnd, ECC_WorldStatic);

	if (bDidHitSomething)
	{
		for (int i = 0; i < lineTraceHits.Num(); i++)
		{
			// Prevent the player from being detected by the line trace.
			// This could probably be optimized to have the line trace 
			// ignore the player in the first place.
			if (lineTraceHits[i].Actor != this)
			{
				lineTraceEnd = lineTraceHits[i].Location;
				FRotator projectileRotation = UKismetMathLibrary::FindLookAtRotation(spawnTM.GetLocation(), lineTraceHits[i].Location);
				spawnTM.SetRotation(projectileRotation.Quaternion());
				break;
			}
		}
	}

	if (gameInstance && gameInstance->DrawDebugInfo)
	{
		DrawDebugLine(GetWorld(), spawnTM.GetLocation(), lineTraceEnd, bDidHitSomething ? FColor::Red : FColor::Green, false, 2.0f, 0, 2.0f);
	}

	FActorSpawnParameters spawnParams;

	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(projectileClass, spawnTM, spawnParams);
}

void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (gameInstance && gameInstance->DrawDebugInfo)
	{
		// -- Rotation Visualization -- //
		const float DrawScale = 100.0f;
		const float Thickness = 5.0f;

		FVector LineStart = GetActorLocation();
		// Offset to the right of pawn
		LineStart += GetActorRightVector() * 100.0f;
		// Set line end in direction of the actor's forward
		FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
		FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
		
		// Draw actor's direction
		DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);
		// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
		DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
	}
}

void ASCharacter::DebugButton()
{
	if (gameInstance)
		gameInstance->DrawDebugInfo = ! gameInstance->DrawDebugInfo;
	else
		gameInstance = Cast<USGameInstance>(GetGameInstance());
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Player controls
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	// Camera controls
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Lookup", this, &APawn::AddControllerPitchInput);
	
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("BlackHoleAttack", IE_Pressed, this, &ASCharacter::BlackHoleAttack);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("DebugButton", IE_Pressed, this, &ASCharacter::DebugButton);
}

