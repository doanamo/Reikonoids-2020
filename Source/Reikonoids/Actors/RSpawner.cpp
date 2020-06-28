#include "RSpawner.h"
#include "../Gameplay/RSpawnDirector.h"

ARSpawner::ARSpawner() = default;
ARSpawner::~ARSpawner() = default;

void ARSpawner::SetupDeferredSpawnRegistration(URSpawnDirector* InSpawnDirector, TArray<AActor*>* InPopulation)
{
    check(InSpawnDirector);
    check(InPopulation);

    SpawnDirector = InSpawnDirector;
    Population = InPopulation;
}

void ARSpawner::BeginPlay()
{
    Super::BeginPlay();

    // Calculate total weight for list of entries.
    float WeightSum = 0.0f;

    for(const auto& Entry : Entries)
    {
        WeightSum += FMath::Max(0.0f, Entry.Weight);
    }

    // Spawn random entry from the list.
    float WeightRandom = FMath::RandRange(0.0f, WeightSum);
    float WeightProbe = 0.0f;

    for(const auto& Entry : Entries)
    {
        if(WeightRandom <= WeightProbe + Entry.Weight)
        {
            // Spawn actor definition.
            FActorSpawnParameters SpawnParams;
            AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(Entry.Class, GetActorTransform(), SpawnParams);

            // Register spawned actor if needed.
            if(SpawnedActor && SpawnDirector)
            {
                Population->Add(SpawnedActor);
            }

            break;
        }

        WeightProbe += Entry.Weight;
    }

    // Destroy spawner.
    Destroy();
}

