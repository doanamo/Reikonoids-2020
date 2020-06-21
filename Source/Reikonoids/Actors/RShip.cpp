#include "RShip.h"
#include "../Components/RHealthComponent.h"
#include "../Components/RWeaponComponent.h"
#include <Components/CapsuleComponent.h>

ARShip::ARShip()
{
    // Use controller rotation.
    bUseControllerRotationPitch = true;
    bUseControllerRotationYaw = true;

    // Setup capsule component.
    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    CapsuleComponent->SetSimulatePhysics(true);
    CapsuleComponent->SetCapsuleRadius(50.0f);
    CapsuleComponent->SetCapsuleHalfHeight(50.0f);
    CapsuleComponent->SetCollisionProfileName(FName("Pawn"));
    CapsuleComponent->SetConstraintMode(EDOFMode::XYPlane);
    CapsuleComponent->SetLinearDamping(LinearDampingForce);
    RootComponent = CapsuleComponent;

    // Setup health component.
    HealthComponent = CreateDefaultSubobject<URHealthComponent>(TEXT("HealthComponent"));
    HealthComponent->OnDeath.AddDynamic(this, &ARShip::OnDeath);

    // Setup weapon component.
    WeaponComponent = CreateDefaultSubobject<URWeaponComponent>(TEXT("WeaponComponent"));
    WeaponComponent->SetupAttachment(CapsuleComponent);
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
    CapsuleComponent->AddImpulse(MovementInput * MovementImpulseSize);
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
