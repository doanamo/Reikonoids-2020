#include "RShip.h"
#include "../Components/RHealthComponent.h"
#include "../Components/RModifierStackComponent.h"
#include "../Components/RWeaponComponent.h"
#include "../Gameplay/RGameMode.h"
#include <Components/SphereComponent.h>
#include <GameFramework/Actor.h>

ARShip::ARShip()
{
    // Use controller rotation.
    bUseControllerRotationPitch = true;
    bUseControllerRotationYaw = true;

    // Auto posses when spawn director spawns this actor.
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

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

    // Create modifier stack component.
    ModifierStack = CreateDefaultSubobject<URModifierStackComponent>(TEXT("ModifierStack"));

    // Setup weapon component.
    WeaponComponent = CreateDefaultSubobject<URWeaponComponent>(TEXT("WeaponComponent"));
    WeaponComponent->SetupAttachment(SphereCollision);
    WeaponComponent->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));
}

ARShip::~ARShip() = default;

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

    // Apply physical movement impulse.
    SphereCollision->AddImpulse(GetActorForwardVector() * MovementImpulseSize * ForwardMovementInput * DeltaTime);

    // Apply controller rotation input.
    AddControllerYawInput(YawRotationInput);
}

void ARShip::FaceRotation(FRotator NewControlRotation, float DeltaTime)
{
    // Smoothly change rotation when adjusting pawn to face controller's rotation.
    SetActorRotation(FMath::RInterpTo(GetActorRotation(), NewControlRotation, DeltaTime, 4.0f));
}

void ARShip::OnDeath()
{
    // Increase score for destroying this ship.
    if(ARGameMode* GameMode = GetWorld()->GetAuthGameMode<ARGameMode>())
    {
        GameMode->PlayerScore += DestructionScore;
    }

    // Destroy actor.
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
    ForwardMovementInput = FMath::Clamp(AxisScale, 0.0f, 1.0f);
}

void ARShip::RotateRight(float AxisScale)
{
    YawRotationInput = FMath::Clamp(AxisScale, -1.0f, 1.0f);
}
