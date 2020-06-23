#pragma once

#include "../RModifier.h"
#include "RModifier_FireRate.generated.h"

class ARShip;

UCLASS()
class REIKONOIDS_API URModifier_FireRate : public URModifier
{
    GENERATED_BODY()

public:
    URModifier_FireRate();
    ~URModifier_FireRate();

    UPROPERTY(EditDefaultsOnly)
    float FireRateScale = 4.0f;

private:
    virtual bool Apply(AActor* ModifiedActor) override;
    virtual void Revert() override;

    ARShip* ModifiedShip = nullptr;
    float PreviousFireRate = 0.0f;
};
