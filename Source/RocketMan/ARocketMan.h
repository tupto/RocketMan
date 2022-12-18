// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "CollisionQueryParams.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"

#include "RocketProjectile.h"

#include "ARocketMan.generated.h"

UCLASS()
class ROCKETMAN_API ARocketMan : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARocketMan();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FTimerHandle FireTimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual bool CanJumpInternal_Implementation() const override;

	//Handles forward/backward movement
	UFUNCTION()
	void MoveForward(float Value);

	//Handles left/right movement
	UFUNCTION()
	void MoveRight(float Value);

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void StopJump();

	UFUNCTION()
	void StartCrouch();

	UFUNCTION()
	void StopCrouch();

	UFUNCTION()
	void StartFire();

	UFUNCTION()
	void StopFire();

	// Function that handles firing projectiles.
	UFUNCTION()
	void Fire();

	UFUNCTION(BlueprintImplementableEvent)
	void OnFire();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* FPSCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	float FireRate;

	// Projectile class to spawn.
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ARocketProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool PressedFire;
};
