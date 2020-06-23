#pragma once

#include "RModifier_Base.h"
#include "RModifier_HealthRegeneration.generated.h"

class ARShip;

UCLASS()
class REIKONOIDS_API URModifier_HealthRegeneration : public URModifier_Base
{
    GENERATED_BODY()

public:
    URModifier_HealthRegeneration();
    ~URModifier_HealthRegeneration();

    UPROPERTY(EditDefaultsOnly)
    float HealthRegenerationPerSecond = 5.0f;

private:
    virtual bool Apply(AActor* ModifiedActor) override;
    virtual void Tick(float DeltaSeconds) override;

    ARShip* ModifiedShip = nullptr;
    float SecondAccumulator = 0.0f;
};
