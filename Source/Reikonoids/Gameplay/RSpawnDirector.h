#pragma once

#include <UObject/Object.h>
#include <Containers/Array.h>
#include <Containers/List.h>
#include "RSpawnDirector.generated.h"

class UWorld;
class AActor;

USTRUCT(BlueprintType)
struct REIKONOIDS_API FRSpawnPopulation
{
    GENERATED_BODY()

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
    TMap<TSubclassOf<AActor>, FRSpawnPopulation> PopulationMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float PopulationUpdateDelay = 1.0f;

    URSpawnDirector();
    ~URSpawnDirector();

    void SetSpawnOrigin(const FVector& InSpawnOrigin);
    void OverrideMinSpawnRadiusOnNextUpdate(float Radius);
    void SetupPopulationUpdate(UWorld* InWorld);
    void RegisterGenericActor(AActor* Actor);

private:
    void UpdatePopulation();

    bool SpawnEnabled = true;
    UWorld* World = nullptr;
    FTimerHandle UpdateTimer;
    FVector SpawnOrigin = FVector::ZeroVector;
    float OverrideIgnoreMinSpawnRadius = -1.0f;
};
