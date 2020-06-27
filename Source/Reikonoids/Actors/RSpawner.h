#pragma once

#include <GameFramework/Actor.h>
#include "RSpawner.generated.h"

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

protected:
    ARSpawner();

    virtual void BeginPlay() override;
};
