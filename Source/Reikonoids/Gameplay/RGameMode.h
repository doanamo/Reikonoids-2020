#pragma once

#include <GameFramework/GameMode.h>
#include "RGameMode.generated.h"

class URSpawnDirector;

UCLASS()
class REIKONOIDS_API ARGameMode : public AGameMode
{
    GENERATED_BODY()

public:
    virtual ~ARGameMode();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
    URSpawnDirector* SpawnDirector = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    int PlayerScore = 0;

protected:
    ARGameMode();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
};
