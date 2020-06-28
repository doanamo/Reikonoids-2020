#include "RSpawner.h"

ARSpawner::ARSpawner() = default;
ARSpawner::~ARSpawner() = default;

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
            FActorSpawnParameters SpawnParams;
            GetWorld()->SpawnActor<AActor>(Entry.Class, GetActorTransform(), SpawnParams);
            break;
        }

        WeightProbe += Entry.Weight;
    }

    // Destroy spawner.
    Destroy();
}

