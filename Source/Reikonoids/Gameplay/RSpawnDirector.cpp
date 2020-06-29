#include "RSpawnDirector.h"
#include "../Actors/RSpawner.h"
#include <GameFramework/Actor.h>
#include <Kismet/GameplayStatics.h>

URSpawnDirector::URSpawnDirector() = default;
URSpawnDirector::~URSpawnDirector() = default;

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
            if(*Actor == nullptr)
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
                // Calculate random spawn location using uniformly randomized point on annulus.
                float SpawnRadiusMin = OverrideIgnoreMinSpawnRadius >= 0.0f ? OverrideIgnoreMinSpawnRadius : Definition.SpawnRadiusMin;
                float SpawnRadiusMax = FMath::Max(SpawnRadiusMin, Definition.SpawnRadiusMax);

                float Theta = FMath::RandRange(0.0f, 360.0f);
                float Distance = FMath::Sqrt(FMath::RandRange(0.0f, 1.0f) * (FMath::Square(SpawnRadiusMin) - FMath::Square(SpawnRadiusMax)) + FMath::Square(SpawnRadiusMax));

                FVector RandomLocation;
                RandomLocation.X = Distance * FMath::Cos(Theta);
                RandomLocation.Y = Distance * FMath::Sin(Theta);
                RandomLocation.Z = 0.0f;
                RandomLocation += SpawnOrigin;

                // Use random rotation if requested.
                FRotator RandomRotation;
                if(Definition.RandomizeRotation)
                {
                    RandomRotation.Yaw = FMath::RandRange(0.0f, 360.0f);
                }

                // Spawn actor.
                // Deferred construction is needed otherwise actors such as spawners that
                // destroy themselves immediatelly will result in SpawnActor() returning nullptr.
                FTransform SpawnTransform;
                SpawnTransform.SetLocation(RandomLocation);
                SpawnTransform.SetRotation(RandomRotation.Quaternion());

                FActorSpawnParameters SpawnParams;
                SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
                SpawnParams.bDeferConstruction = true;

                if(AActor* SpawnedActor = World->SpawnActor<AActor>(Definition.ActorClass, SpawnTransform, SpawnParams))
                {
                    if(GEngine)
                    {
                        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow,
                            FString::Printf(TEXT("Spawn director spawned %s at distance of %f units"),
                            *SpawnedActor->GetName(), FVector::Dist(SpawnOrigin, RandomLocation)));
                    }

                    if(ARSpawner* SpawnerActor = Cast<ARSpawner>(SpawnedActor))
                    {
                        // Defer actor registration to spawner as it will immediately destroy
                        // itself after spawning an actor which would otherwise not be registered.
                        // Passing population array is fine as long as we do not allow removal of populations.
                        SpawnerActor->SetupDeferredSpawnRegistration(this, &Population.Actors);
                    }
                    else
                    {
                        // Register actor in population list.
                        Population.Actors.Push(SpawnedActor);
                    }

                    UGameplayStatics::FinishSpawningActor(SpawnedActor, SpawnTransform);
                }
            }
        }
    }

    // Toggle minimum spawn radius override off after update.
    OverrideIgnoreMinSpawnRadius = -1.0f;
}
