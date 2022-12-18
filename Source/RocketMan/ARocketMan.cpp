// Fill out your copyright notice in the Description page of Project Settings.


#include "ARocketMan.h"

// Sets default values
ARocketMan::ARocketMan()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a first person camera component.
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FPSCameraComponent != nullptr);

	// Attach the camera component to our capsule component.
	FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent>(GetCapsuleComponent()));

	// Position the camera slightly above the eyes.
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));

	FPSCameraComponent->bUsePawnControlRotation = true;
	PressedFire = false;
	FireRate = 0.6f;

}

// Called when the game starts or when spawned
void ARocketMan::BeginPlay()
{
	Super::BeginPlay();
	
	check(GEngine != nullptr);
}

// Called every frame
void ARocketMan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CameraLocation;
	FRotator CameraRotation;
	GetActorEyesViewPoint(CameraLocation, CameraRotation);

	FVector FocalPoint;
	FVector LineTraceEnd = CameraLocation + (CameraRotation.Vector() * 10000.0f);
	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;

	// Skew the aim to be slightly upwards.
	FRotator MuzzleRotation = CameraRotation;

	if (ActorLineTraceSingle(OutHit, CameraLocation, LineTraceEnd, ECC_WorldStatic, CollisionParams))
	{
		FocalPoint = OutHit.ImpactPoint;
	}
	else
	{
		FocalPoint = LineTraceEnd;
	}

	//if (LookAtPoint)
	//	LookAtPoint->SetWorldLocation(FocalPoint);
}

// Called to bind functionality to input
void ARocketMan::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ARocketMan::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ARocketMan::MoveRight);


	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &ARocketMan::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &ARocketMan::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARocketMan::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ARocketMan::StopJump);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ARocketMan::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ARocketMan::StopCrouch);

	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &ARocketMan::StartFire);
	PlayerInputComponent->BindAction("PrimaryAction", IE_Released, this, &ARocketMan::StopFire);

}

bool ARocketMan::CanJumpInternal_Implementation() const
{
	bool base = ACharacter::CanJumpInternal_Implementation();
	if (!base)
	{
		if (bIsCrouched && !GetCharacterMovement()->IsFalling())
		{
			return true;
		}
	}
	return base;
}

void ARocketMan::MoveForward(float Value)
{
	// Find out which way is "forward" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	Direction = FVector(Direction.X, Direction.Y, 0.0f).GetSafeNormal();
	AddMovementInput(Direction, Value);
}

void ARocketMan::MoveRight(float Value)
{
	// Find out which way is "right" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}


void ARocketMan::StartJump()
{
	Jump();
}

void ARocketMan::StopJump()
{
	StopJumping();
}

void ARocketMan::StartCrouch()
{
	Crouch();
}

void ARocketMan::StopCrouch()
{
	UnCrouch();
}

void ARocketMan::StartFire()
{
	PressedFire = true;
	if (!GetWorldTimerManager().IsTimerActive(FireTimerHandle))
	{
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ARocketMan::Fire, FireRate, true, 0.0f);
	}
}

void ARocketMan::StopFire()
{
	PressedFire = false;
}


void ARocketMan::Fire()
{
	if (PressedFire && ProjectileClass)
	{
		OnFire();

		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// Skew the aim to be slightly upwards.
		FRotator MuzzleRotation = CameraRotation;

		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// Spawn the projectile at the muzzle.
			ARocketProjectile* Projectile = World->SpawnActor<ARocketProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				Projectile->FireInDirection(CameraRotation.Vector());
				
			}
		}
	}
	else if (!PressedFire)
	{
		GetWorldTimerManager().ClearTimer(FireTimerHandle);
	}
}