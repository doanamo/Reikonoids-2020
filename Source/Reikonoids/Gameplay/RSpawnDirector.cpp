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

void URSpawnDirector::SetupPopulationUpdate(UWorld* InWorld)
{
    // Save reference to world in which we will spawn actors.
    World = InWorld;

    // Setup population update at constant rate.
    FTimerManager& TimerManager = GetWorld()->GetTimerManager();
    TimerManager.SetTimer(UpdateTimer, this, &URSpawnDirector::UpdatePopulation, PopulationUpdateDelay, true, 0.0f);
}

void URSpawnDirector::SetSpawnOrigin(const FVector& InSpawnOrigin)
{
    SpawnOrigin = InSpawnOrigin;
}

void URSpawnDirector::RegisterGenericActor(AActor* Actor)
{
    check(Actor);

    // Find generic population by actor class.
    FRSpawnPopulation* Population = PopulationMap.Find(AActor::StaticClass());

    if(!Population)
    {
#ifdef WITH_EDITOR
        if(GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red,
                FString::Printf(TEXT("Failed to register generic actor %s due to missing population"),
                *Actor->GetName()));
        }
#endif

        return;
    }

    // Add actor to generic population list.
    Population->Actors.Add(Actor);
}

void URSpawnDirector::UpdatePopulation()
{
    check(World != nullptr);

    // Iterate over populations.
    for(auto& SpawnPopulationEntry : PopulationMap)
    {
        const TSubclassOf<AActor>& ActorClass = SpawnPopulationEntry.Key;
        FRSpawnPopulation& SpawnPopulation = SpawnPopulationEntry.Value;

        // Remove actors that have been destroyed.
        for(auto Actor = SpawnPopulation.Actors.CreateIterator(); Actor; Actor++)
        {
            if(*Actor == nullptr)
            {
                Actor.RemoveCurrent();
            }
        }

        // Despawn actors that are past despawn radius.
        for(auto Actor = SpawnPopulation.Actors.CreateIterator(); Actor; Actor++)
        {
            float DistanceSqr = FVector::DistSquared(SpawnOrigin, (*Actor)->GetActorLocation());

            if(DistanceSqr > SpawnPopulation.DespawnRadius * SpawnPopulation.DespawnRadius)
            {
                (*Actor)->Destroy();
                Actor.RemoveCurrent();
            }
        }

        // Spawn actors within spawn radius.
        if(SpawnEnabled)
        {
            int SpawnsNeeded = SpawnPopulation.SpawnCount - SpawnPopulation.Actors.Num();
            for(int SpawnIndex = 0; SpawnIndex < SpawnsNeeded; ++SpawnIndex)
            {
                // Calculate random spawn location using uniformly randomized point on annulus.
                float SpawnRadiusMin = OverrideIgnoreMinSpawnRadius >= 0.0f ? OverrideIgnoreMinSpawnRadius : SpawnPopulation.SpawnRadiusMin;
                float SpawnRadiusMax = FMath::Max(SpawnRadiusMin, SpawnPopulation.SpawnRadiusMax);

                float Theta = FMath::RandRange(0.0f, 360.0f);
                float Distance = FMath::Sqrt(FMath::RandRange(0.0f, 1.0f) * (FMath::Square(SpawnRadiusMin) - FMath::Square(SpawnRadiusMax)) + FMath::Square(SpawnRadiusMax));

                FVector RandomLocation;
                RandomLocation.X = Distance * FMath::Cos(Theta);
                RandomLocation.Y = Distance * FMath::Sin(Theta);
                RandomLocation.Z = 0.0f;
                RandomLocation += SpawnOrigin;

                // Use random rotation if requested.
                FRotator RandomRotation;
                if(SpawnPopulation.RandomizeRotation)
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

                if(AActor* SpawnedActor = World->SpawnActor<AActor>(ActorClass, SpawnTransform, SpawnParams))
                {
#ifdef WITH_EDITOR
                    if(GEngine)
                    {
                        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow,
                            FString::Printf(TEXT("Spawn director spawned %s at distance of %f units"),
                            *SpawnedActor->GetName(), FVector::Dist(SpawnOrigin, RandomLocation)));
                    }
#endif

                    if(ARSpawner* SpawnerActor = Cast<ARSpawner>(SpawnedActor))
                    {
                        // Defer actor registration to spawner as it will immediately destroy
                        // itself after spawning an actor which would otherwise not be registered.
                        // Passing population array is fine as long as we do not allow removal of populations.
                        SpawnerActor->SetupDeferredSpawnRegistration(this, &SpawnPopulation.Actors);
                    }
                    else
                    {
                        // Register actor in population list.
                        SpawnPopulation.Actors.Push(SpawnedActor);
                    }

                    UGameplayStatics::FinishSpawningActor(SpawnedActor, SpawnTransform);
                }
            }
        }
    }

    // Toggle minimum spawn radius override off after update.
    OverrideIgnoreMinSpawnRadius = -1.0f;
}
