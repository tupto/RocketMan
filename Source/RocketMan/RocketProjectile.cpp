// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketProjectile.h"

// Sets default values
ARocketProjectile::ARocketProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}

	if (!CollisionComponent)
	{
		// Use a sphere as a simple collision representation.
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		// Set the sphere's collision radius.
		CollisionComponent->InitSphereRadius(5.0f);
		// Set the sphere's collision profile name to "Projectile".
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
		// Set the root component to be the collision component.
		RootComponent = CollisionComponent;
	}

	if (!ProjectileMovementComponent)
	{
		// Use this component to drive this projectile's movement.
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed = 995.5f;
		ProjectileMovementComponent->MaxSpeed = 995.5f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}

	if (!MeshComponent)
	{
		MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
		//MeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		MeshComponent->SetupAttachment(RootComponent);
	}

	if (!ImpactEmitter)
	{
		//ImpactEmitter = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ImpactEmitter"));
		//ImpactEmitter->SetupAttachment(RootComponent);
		//MeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}

	ExplosionRadius = 300.0;
	ExplosionForce = 500.0;
}

// Called when the game starts or when spawned
void ARocketProjectile::BeginPlay()
{
	Super::BeginPlay();
	MeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//ImpactEmitter->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Event called when component hits something.
	CollisionComponent->OnComponentHit.AddDynamic(this, &ARocketProjectile::OnHit);
}

// Called every frame
void ARocketProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TrailEmitter, GetActorLocation());

}

// Function that initializes the projectile's velocity in the shoot direction.
void ARocketProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void ARocketProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	OnExplode();

	if (OtherActor != nullptr && OtherComponent != nullptr && OtherComponent->IsSimulatingPhysics())
	{
		OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
	}

	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEmitter, Hit.ImpactPoint);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraImpact, Hit.ImpactPoint);

	TArray<FHitResult> OutHits;
	FVector MyLocation = GetActorLocation();
	FCollisionShape MyColSphere = FCollisionShape::MakeSphere(ExplosionRadius);
	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, MyLocation, MyLocation, FQuat::Identity, ECC_WorldStatic, MyColSphere);

	if (isHit)
	{
		// loop through TArray
		for (auto& Hit : OutHits)
		{
			UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>((Hit.GetActor())->GetRootComponent());

			if (MeshComp)
			{
				// alternivly you can use  ERadialImpulseFalloff::RIF_Linear for the impulse to get linearly weaker as it gets further from origin.
				// set the float radius to 500 and the float strength to 2000.
				MeshComp->AddRadialImpulse(GetActorLocation(), ExplosionRadius, ExplosionForce*2, ERadialImpulseFalloff::RIF_Constant, true);
			}
			else
			{

				ACharacter* Char = Cast<ACharacter>(Hit.GetActor());
				if (Char)
				{
					// alternivly you can use  ERadialImpulseFalloff::RIF_Linear for the impulse to get linearly weaker as it gets further from origin.
					// set the float radius to 500 and the float strength to 2000.
					Char->GetCharacterMovement()->AddRadialImpulse(GetActorLocation(), ExplosionRadius, ExplosionForce, ERadialImpulseFalloff::RIF_Constant, true);

					//FVector dir = GetActorLocation() - Hit.GetActor()->GetActorLocation();
					//dir.Normalize();
					//CapComp->AddImpulse(dir * 8000.0f, NAME_None, true);
				}
			}
		}
	}

	Destroy();
}