#include "RAsteroid.h"
#include "../Components/RHealthComponent.h"
#include "../Gameplay/RGameMode.h"
#include "../Gameplay/RSpawnDirector.h"
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

ARAsteroid::~ARAsteroid() = default;

void ARAsteroid::BeginPlay()
{
    Super::BeginPlay();

    if(ApplyRandomTorque)
    {
        FVector RandomTorque = RandomizeTorque();
        SphereCollision->AddAngularImpulseInDegrees(RandomTorque, NAME_None, true);
    }

    if(ApplyRandomVelocity)
    {
        FVector RandomVelocity = RandomizeVelocity();
        SphereCollision->AddImpulse(RandomVelocity, NAME_None, true);
    }
}

FVector ARAsteroid::RandomizeTorque() const
{
    FVector RandomTorque;
    RandomTorque.X = FMath::RandRange(-MaxRandomTorque.X, MaxRandomTorque.X);
    RandomTorque.Y = FMath::RandRange(-MaxRandomTorque.Y, MaxRandomTorque.Y);
    RandomTorque.Z = FMath::RandRange(-MaxRandomTorque.Z, MaxRandomTorque.Z);
    return RandomTorque;
}

FVector ARAsteroid::RandomizeVelocity() const
{
    FVector RandomVelocity;
    RandomVelocity.X = FMath::RandRange(-MaxRandomVelocity.X, MaxRandomVelocity.X);
    RandomVelocity.Y = FMath::RandRange(-MaxRandomVelocity.Y, MaxRandomVelocity.Y);
    RandomVelocity.Z = FMath::RandRange(-MaxRandomVelocity.Z, MaxRandomVelocity.Z);
    return RandomVelocity;
}

void ARAsteroid::OnDeath()
{
    // Get current velocity before we destroy this actor.
    // It gets immediately cleared to zeros when destroying it.
    FVector CurrentVelocity = SphereCollision->GetComponentVelocity();

    // Destroy asteroid actor before fracturing.
    // Should probably reuse this one for one fracture.
    Destroy();

    // Create fractured asteroids.
    if(FractureIndex == 0)
        return;

    for(int i = 0; i < FractureCount; ++i)
    {
        // Randomize spawn point.
        FVector2D RandomPointInCircle = FMath::RandPointInCircle(32.0f);

        FVector RandomLocation;
        RandomLocation.X = RandomPointInCircle.X;
        RandomLocation.Y = RandomPointInCircle.Y;
        RandomLocation.Z = 0.0f;
        RandomLocation += GetActorLocation();

        // Randomize velocity modifier.
        FVector ModifiedVelocity = CurrentVelocity * 0.6f + RandomizeVelocity() * 0.4f;

        // Spawn asteroid.
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        SpawnParams.bDeferConstruction = true;

        ARAsteroid* Asteroid = GetWorld()->SpawnActor<ARAsteroid>(this->GetClass(), SpawnParams);

        // Register fractured asteroid in spawn director so its lifetime is managed.
        if(ARGameMode* GameMode = GetWorld()->GetAuthGameMode<ARGameMode>())
        {
            GameMode->SpawnDirector->RegisterGenericActor(Asteroid);
        }

        // Setup asteroid.
        Asteroid->FractureIndex = FractureIndex - 1;
        Asteroid->FractureCount = FractureCount + 1;
        Asteroid->SphereCollision->SetSphereRadius(SphereCollision->GetUnscaledSphereRadius() * FractureScale);
        Asteroid->SphereCollision->SetMassScale(NAME_None, SphereCollision->GetMassScale() * FractureScale);
        Asteroid->StaticMesh->SetWorldScale3D(StaticMesh->GetComponentScale() * FractureScale);
        Asteroid->HealthComponent->MaximumHealth = HealthComponent->MaximumHealth * FractureScale;

        Asteroid->ApplyRandomVelocity = false;
        Asteroid->SphereCollision->AddImpulse(ModifiedVelocity, NAME_None, true);

        FTransform SpawnTranform;
        SpawnTranform.SetLocation(RandomLocation);
        UGameplayStatics::FinishSpawningActor(Asteroid, SpawnTranform);
    }
}
