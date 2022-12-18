// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "NiagaraFunctionLibrary.h"

#include "RocketProjectile.generated.h" 	


UCLASS()
class ROCKETMAN_API ARocketProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARocketProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function that initializes the projectile's velocity in the shoot direction.
	void FireInDirection(const FVector& ShootDirection);

	// Function that is called when the projectile hits something.
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	// Sphere collision component.
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;

	UFUNCTION(BlueprintImplementableEvent)
	void OnExplode();

	// Mesh component.
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	class UNiagaraSystem* NiagaraImpact;

	// Particle class to spawn.
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	class UParticleSystem* ImpactEmitter;

	// Particle class to spawn.
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	class UParticleSystem* TrailEmitter;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	double ExplosionRadius;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	double ExplosionForce;

	// Projectile movement component.
	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;

};
