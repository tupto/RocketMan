// Fill out your copyright notice in the Description page of Project Settings.


#include "ARocketLauncher.h"

// Sets default values
ARocketLauncher::ARocketLauncher()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->InitSphereRadius(32.0f);
	Sphere->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	RootComponent = Mesh;

}

// Called when the game starts or when spawned
void ARocketLauncher::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARocketLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARocketLauncher::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ARocketMan::StaticClass()))
	{
	}
}

void ARocketLauncher::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

