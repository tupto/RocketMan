// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ARocketMan.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "ARocketLauncher.generated.h"

UCLASS()
class ROCKETMAN_API ARocketLauncher : public AActor
{
	GENERATED_BODY()
	
public:	
	// Mesh Component
	UPROPERTY(VisibleAnywhere, Category = "Launcher Components")
	class UStaticMeshComponent* Mesh;

	// Sphere Component
	UPROPERTY(VisibleAnywhere, Category = "Launcher Components")
	class USphereComponent* Sphere;

	// Sets default values for this actor's properties
	ARocketLauncher();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** called; when something enters the sphere component */
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** called when something leaves the sphere component */
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
