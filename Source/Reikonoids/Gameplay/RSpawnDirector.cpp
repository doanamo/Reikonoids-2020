#include "RSpawnDirector.h"
#include <GameFramework/Actor.h>

URSpawnDirector::URSpawnDirector()
{
}

void URSpawnDirector::ToggleSpawning(bool Enabled)
{
    SpawnEnabled = Enabled;
}

void URSpawnDirector::OverrideMinSpawnRadiusOnNextUpdate(float Radius)
{
    OverrideIgnoreMinSpawnRadius = Radius;
}

void URSpawnDirector::SetupPopulationUpdate(UWorld* DirectedWorld)
{
    // Save reference to world in which we will spawn actors.
    World = DirectedWorld;

    // Create population list based on spawn definitions.
    for(auto& SpawnDefinition : SpawnDefinitions)
    {
        PopulationList.Emplace();
    }

    // Setup population update at constant rate.
    FTimerManager& TimerManager = GetWorld()->GetTimerManager();
    TimerManager.SetTimer(UpdateTimer, this, &URSpawnDirector::UpdatePopulation, PopulationUpdateDelay, true, 0.0f);
}

void URSpawnDirector::SetSpawnOrigin(const FVector& NewSpawnOrigin)
{
    SpawnOrigin = NewSpawnOrigin;
}

void URSpawnDirector::UpdatePopulation()
{
    check(World != nullptr);

    // Iterate over populations.
    check(PopulationList.Num() == SpawnDefinitions.Num());
    for(int DefinitionIndex = 0; DefinitionIndex < SpawnDefinitions.Num(); ++DefinitionIndex)
    {
        const FRSpawnDefinition& Definition = SpawnDefinitions[DefinitionIndex];
        FRSpawnPopulation& Population = PopulationList[DefinitionIndex];

        // Remove actors that have been destroyed.
        for(auto Actor = Population.Actors.CreateIterator(); Actor; Actor++)
        {
            if(*Actor == nullptr || (*Actor)->IsActorBeingDestroyed())
            {
                Actor.RemoveCurrent();
            }
        }

        // Despawn actors that are past despawn radius.
        for(auto Actor = Population.Actors.CreateIterator(); Actor; Actor++)
        {
            float DistanceSqr = FVector::DistSquared(SpawnOrigin, (*Actor)->GetActorLocation());

            if(DistanceSqr > Definition.DespawnRadius * Definition.DespawnRadius)
            {
                (*Actor)->Destroy();
                Actor.RemoveCurrent();
            }
        }

        // Spawn actors within spawn radius.
        if(SpawnEnabled)
        {
            int SpawnsNeeded = Definition.SpawnCount - Population.Actors.Num();
            for(int SpawnIndex = 0; SpawnIndex < SpawnsNeeded; ++SpawnIndex)
            {
                // Calculate random position using uniformly randomized point on annulus.
                float SpawnRadiusMin = OverrideIgnoreMinSpawnRadius >= 0.0f ? OverrideIgnoreMinSpawnRadius : Definition.SpawnRadiusMin;
                float SpawnRadiusMax = FMath::Max(SpawnRadiusMin, Definition.SpawnRadiusMax);

                float Theta = FMath::RandRange(0.0f, 360.0f);
                float Distance = FMath::Sqrt(FMath::RandRange(0.0f, 1.0f) * (FMath::Square(SpawnRadiusMin) - FMath::Square(SpawnRadiusMax)) + FMath::Square(SpawnRadiusMax));

                FVector RandomPosition;
                RandomPosition.X = Distance * FMath::Cos(Theta);
                RandomPosition.Y = Distance * FMath::Sin(Theta);
                RandomPosition += SpawnOrigin;

                // Use random rotation if requested.
                FRotator RandomRotation;
                if(Definition.RandomizeRotation)
                {
                    RandomRotation.Yaw = FMath::RandRange(0.0f, 360.0f);
                }

                // Spawn actor.
                FActorSpawnParameters SpawnParams;
                SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

                if(AActor* Actor = World->SpawnActor<AActor>(Definition.ActorClass, RandomPosition, RandomRotation, SpawnParams))
                {
                    if(GEngine)
                    {
                        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow,
                            FString::Printf(TEXT("Spawn director spawned %s at distance of %f units"),
                                *Actor->GetName(), FVector::Dist(SpawnOrigin, RandomPosition)));
                    }

                    Population.Actors.Push(Actor);

                    check(FVector::Dist(SpawnOrigin, Actor->GetActorLocation()) > SpawnRadiusMin);
                }
            }
        }
    }

    // Toggle minimum spawn radius override off after update.
    OverrideIgnoreMinSpawnRadius = -1.0f;
}
