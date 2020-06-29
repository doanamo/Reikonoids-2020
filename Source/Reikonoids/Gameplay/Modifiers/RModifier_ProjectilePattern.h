#pragma once

#include "RModifier_Base.h"
#include "../../Components/RWeaponComponent.h"
#include "RModifier_ProjectilePattern.generated.h"

class ARShip;

UCLASS()
class REIKONOIDS_API URModifier_ProjectilePattern : public URModifier_Base
{
    GENERATED_BODY()

public:
    URModifier_ProjectilePattern();
    ~URModifier_ProjectilePattern();

    UPROPERTY(EditDefaultsOnly)
    FRWeaponFiringPattern FiringPattern;

private:
    virtual bool Apply(AActor* ModifiedActor) override;
    virtual void Revert() override;

    ARShip* ModifiedShip = nullptr;
    FRWeaponFiringPattern PreviousFiringPattern;
};
