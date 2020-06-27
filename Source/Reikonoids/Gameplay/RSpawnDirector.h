#pragma once

#include <UObject/Object.h>
#include <Containers/Array.h>
#include <Containers/List.h>
#include "RSpawnDirector.generated.h"

class UWorld;
class AActor;

USTRUCT(BlueprintType)
struct FRSpawnDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<AActor> ActorClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float SpawnRadiusMin = 4000.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float SpawnRadiusMax = 6000.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float DespawnRadius = 7000.0f;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int SpawnCount = 100;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool RandomizeRotation = false;
};

struct FRSpawnPopulation
{
    TArray<AActor*> Actors;
};

UCLASS()
class REIKONOIDS_API URSpawnDirector : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FRSpawnDefinition> SpawnDefinitions;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float PopulationUpdateDelay = 1.0f;

    URSpawnDirector();

    void SetSpawnOrigin(const FVector& NewSpawnOrigin);
    void SetupPopulationUpdate(UWorld* DirectedWorld);
    void OverrideMinSpawnRadiusOnNextUpdate(float Radius);

private:
    void UpdatePopulation();

    UWorld* World = nullptr;
    FTimerHandle UpdateTimer;
    FVector SpawnOrigin = FVector::ZeroVector;
    TArray<FRSpawnPopulation> PopulationList;
    float OverrideIgnoreMinSpawnRadius = -1.0f;
};
