#pragma once

#include <GameFramework/Actor.h>
#include "RSpawner.generated.h"

class URSpawnDirector;

USTRUCT(BlueprintType)
struct REIKONOIDS_API FRSpawnerEntry
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<AActor> Class;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float Weight = 1.0f;
};

UCLASS()
class REIKONOIDS_API ARSpawner : public AActor
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FRSpawnerEntry> Entries;

    virtual ~ARSpawner();

    void SetupDeferredSpawnRegistration(URSpawnDirector* InSpawnDirector, TArray<AActor*>* InPopulation);

protected:
    ARSpawner();

    virtual void BeginPlay() override;

private:
    URSpawnDirector* SpawnDirector = nullptr;
    TArray<AActor*>* Population = nullptr;
};
