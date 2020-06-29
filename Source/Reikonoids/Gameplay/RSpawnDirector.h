#pragma once

#include <UObject/Object.h>
#include <Containers/Array.h>
#include <Containers/List.h>
#include "RSpawnDirector.generated.h"

class UWorld;
class AActor;

USTRUCT(BlueprintType)
struct REIKONOIDS_API FRSpawnDefinition
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

USTRUCT()
struct REIKONOIDS_API FRSpawnPopulation
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<AActor*> Actors;
};

UCLASS()
class REIKONOIDS_API URSpawnDirector : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void ToggleSpawning(bool Enabled);

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FRSpawnDefinition> SpawnDefinitions;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float PopulationUpdateDelay = 1.0f;

    URSpawnDirector();
    ~URSpawnDirector();

    void OverrideMinSpawnRadiusOnNextUpdate(float Radius);
    void SetupPopulationUpdate(UWorld* DirectedWorld);
    void SetSpawnOrigin(const FVector& NewSpawnOrigin);

private:
    void UpdatePopulation();

    bool SpawnEnabled = true;
    UWorld* World = nullptr;
    FTimerHandle UpdateTimer;
    FVector SpawnOrigin = FVector::ZeroVector;
    float OverrideIgnoreMinSpawnRadius = -1.0f;

    UPROPERTY()
    TArray<FRSpawnPopulation> PopulationList;
};
