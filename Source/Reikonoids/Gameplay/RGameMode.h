#pragma once

#include <GameFramework/GameMode.h>
#include "RGameMode.generated.h"

class URSpawnDirector;

UCLASS()
class REIKONOIDS_API ARGameMode : public AGameMode
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
    URSpawnDirector* SpawnDirector = nullptr;

protected:
    ARGameMode();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
};
