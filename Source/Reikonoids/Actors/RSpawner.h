#pragma once

#include <GameFramework/Actor.h>
#include "RSpawner.generated.h"

class URSpawnDirector;

USTRUCT(BlueprintType)
struct REIKONOIDS_API FRSpawnerEntry
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<AActor> Class;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float Weight = 1.0f;
};

UCLASS()
class REIKONOIDS_API ARSpawner : public AActor
{
    GENERATED_BODY()

public:
    virtual ~ARSpawner();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FRSpawnerEntry> Entries;

    void SetupDeferredSpawnRegistration(URSpawnDirector* InSpawnDirector, TArray<AActor*>* InPopulation);

protected:
    ARSpawner();

    virtual void BeginPlay() override;

private:
    // Below pointers to properties should remain
    // valid as long as global game mode exists.
    URSpawnDirector* SpawnDirector = nullptr;
    TArray<AActor*>* Population = nullptr;
};
