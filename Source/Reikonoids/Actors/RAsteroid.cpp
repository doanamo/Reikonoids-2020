#include "RAsteroid.h"
#include "../Components/RHealthComponent.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Kismet/GameplayStatics.h>

ARAsteroid::ARAsteroid()
{
    // Setup sphere collision component.
    SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    SphereCollision->SetSimulatePhysics(true);
    SphereCollision->SetSphereRadius(100.0f);
    SphereCollision->SetCollisionProfileName(FName("BlockAllDynamic"));
    SphereCollision->BodyInstance.bLockZTranslation = true;
    RootComponent = SphereCollision;

    // Setup mesh component.
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetCollisionProfileName(FName("NoCollision"));
    StaticMesh->SetupAttachment(SphereCollision);

    // Setup health component.
    HealthComponent = CreateDefaultSubobject<URHealthComponent>(TEXT("HealthComponent"));
    HealthComponent->OnDeath.AddDynamic(this, &ARAsteroid::OnDeath);
}

void ARAsteroid::BeginPlay()
{
    Super::BeginPlay();

    ApplyRandomTorque();
}

void ARAsteroid::ApplyRandomTorque()
{
    FVector RandomTorque;
    RandomTorque.X = FMath::RandRange(-MaxRandomTorque.X, MaxRandomTorque.X);
    RandomTorque.Y = FMath::RandRange(-MaxRandomTorque.Y, MaxRandomTorque.Y);
    RandomTorque.Z = FMath::RandRange(-MaxRandomTorque.Z, MaxRandomTorque.Z);

    SphereCollision->AddAngularImpulseInDegrees(RandomTorque, NAME_None, true);
}

void ARAsteroid::ApplyRandomVelocity()
{
    FVector RandomVelocity;
    RandomVelocity.X = FMath::RandRange(-MaxRandomVelocity.X, MaxRandomVelocity.X);
    RandomVelocity.Y = FMath::RandRange(-MaxRandomVelocity.Y, MaxRandomVelocity.Y);
    RandomVelocity.Z = FMath::RandRange(-MaxRandomVelocity.Z, MaxRandomVelocity.Z);

    SphereCollision->AddImpulse(RandomVelocity, NAME_None, true);
}

void ARAsteroid::OnDeath()
{
    // Destroy asteroid actor before fracturing.
    // Should probably reuse this one for one fracture.
    Destroy();

    // Create fractured asteroids.
    if(FractureIndex == 0)
        return;

    for(int i = 0; i < FractureCount; ++i)
    {
        // Spawn asteroid.
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        SpawnParams.bDeferConstruction = true;

        ARAsteroid* Asteroid = GetWorld()->SpawnActor<ARAsteroid>(this->GetClass(), SpawnParams);

        // Setup asteroid.
        Asteroid->FractureIndex = FractureIndex - 1;
        Asteroid->SphereCollision->SetSphereRadius(SphereCollision->GetUnscaledSphereRadius() * FractureScale);
        Asteroid->StaticMesh->SetWorldScale3D(StaticMesh->GetComponentScale() * FractureScale);
        Asteroid->ApplyRandomVelocity();

        FTransform SpawnTranform;
        SpawnTranform.SetLocation(GetActorLocation());
        UGameplayStatics::FinishSpawningActor(Asteroid, SpawnTranform);
    }
}
