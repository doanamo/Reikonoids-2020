#pragma once

#include "RModifier_Base.h"
#include "RModifier_WeaponFireRate.generated.h"

class ARShip;

UCLASS()
class REIKONOIDS_API URModifier_WeaponFireRate : public URModifier_Base
{
    GENERATED_BODY()

public:
    URModifier_WeaponFireRate();
    ~URModifier_WeaponFireRate();

    UPROPERTY(EditDefaultsOnly)
    float FireRateScale = 4.0f;

private:
    virtual bool Apply(AActor* ModifiedActor) override;
    virtual void Revert() override;

    ARShip* ModifiedShip = nullptr;
    float PreviousFireRate = 0.0f;
};
