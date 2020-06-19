#include "RShip.h"
#include "RProjectile.h"
#include "../Components/RHealthComponent.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>

ARShip::ARShip()
{
    // Setup capsule component.
    UCapsuleComponent* Capsule = GetCapsuleComponent();
    check(Capsule != nullptr);

    Capsule->SetCapsuleRadius(50.0f);
    Capsule->SetCapsuleHalfHeight(50.0f);

    // Setup movement component.
    UCharacterMovementComponent* Movement = GetCharacterMovement();
    check(Movement != nullptr);

    Movement->DefaultLandMovementMode = EMovementMode::MOVE_Flying;
    Movement->SetPlaneConstraintNormal(FVector::UpVector);
    Movement->bConstrainToPlane = true;

    // Setup health component.
    HealthComponent = CreateDefaultSubobject<URHealthComponent>(TEXT("HealthComponent"));
    HealthComponent->OnDeath.AddDynamic(this, &ARShip::OnDeath);
}

void ARShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Setup player bindings.
    PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &ARShip::Fire);
    PlayerInputComponent->BindAxis("MoveForward", this, &ARShip::MoveForward);
    PlayerInputComponent->BindAxis("RotateRight", this, &ARShip::RotateRight);
}

void ARShip::BeginPlay()
{
    Super::BeginPlay();
}

void ARShip::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ARShip::OnDeath()
{
    // Destroy on death.
    Destroy();
}

void ARShip::Fire()
{
    check(ProjectileClass != nullptr);

    // Spawn projectile.
    FActorSpawnParameters SpawnParams;
    SpawnParams.Instigator = this;

    FVector ProjectileLocation = GetActorLocation() + GetActorForwardVector() * 100.0f;
    FRotator ProjectileRotation = GetActorRotation();

    ARProjectile* Projectile = GetWorld()->SpawnActor<ARProjectile>(
        ProjectileClass, ProjectileLocation, ProjectileRotation, SpawnParams);

    Projectile->Direction = GetActorForwardVector();
}

void ARShip::MoveForward(float AxisScale)
{
    // Allow only forward movement.
    if(AxisScale > 0.0f)
    {
        GetCharacterMovement()->AddInputVector(GetActorForwardVector() * AxisScale);
    }
}

void ARShip::RotateRight(float AxisScale)
{
    AddControllerYawInput(AxisScale);
}
