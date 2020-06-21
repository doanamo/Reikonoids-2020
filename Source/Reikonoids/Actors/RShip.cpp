#include "RShip.h"
#include "../Components/RHealthComponent.h"
#include "../Components/RWeaponComponent.h"
#include <Components/SphereComponent.h>

ARShip::ARShip()
{
    // Use controller rotation.
    bUseControllerRotationPitch = true;
    bUseControllerRotationYaw = true;

    // Setup sphere collision component.
    SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    SphereCollision->SetSimulatePhysics(true);
    SphereCollision->SetSphereRadius(50.0f);
    SphereCollision->SetConstraintMode(EDOFMode::XYPlane);
    SphereCollision->SetCollisionProfileName(FName("Pawn"));
    SphereCollision->SetLinearDamping(LinearDampingForce);
    RootComponent = SphereCollision;

    // Setup health component.
    HealthComponent = CreateDefaultSubobject<URHealthComponent>(TEXT("HealthComponent"));
    HealthComponent->OnDeath.AddDynamic(this, &ARShip::OnDeath);

    // Setup weapon component.
    WeaponComponent = CreateDefaultSubobject<URWeaponComponent>(TEXT("WeaponComponent"));
    WeaponComponent->SetupAttachment(SphereCollision);
    WeaponComponent->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));
}

void ARShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Setup player bindings.
    PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &ARShip::StartFiring);
    PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &ARShip::StopFiring);
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

    // Apply physical movement.
    FVector MovementInput = ConsumeMovementInputVector();
    SphereCollision->AddImpulse(MovementInput * MovementImpulseSize);
}

void ARShip::OnDeath()
{
    Destroy();
}

void ARShip::StartFiring()
{
    WeaponComponent->StartFiring();
}

void ARShip::StopFiring()
{
    WeaponComponent->StopFiring();
}

void ARShip::MoveForward(float AxisScale)
{
    // Allow only forward movement.
    if(AxisScale > 0.0f)
    {
        AddMovementInput(GetActorForwardVector() * AxisScale);
    }
}

void ARShip::RotateRight(float AxisScale)
{
    AddControllerYawInput(AxisScale);
}
