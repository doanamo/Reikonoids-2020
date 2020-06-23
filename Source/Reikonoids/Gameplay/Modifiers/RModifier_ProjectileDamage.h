#pragma once

#include "RModifier_Base.h"
#include "RModifier_ProjectileDamage.generated.h"

class ARShip;

UCLASS()
class REIKONOIDS_API URModifier_ProjectileDamage : public URModifier_Base
{
    GENERATED_BODY()

public:
    URModifier_ProjectileDamage();
    ~URModifier_ProjectileDamage();

    UPROPERTY(EditDefaultsOnly)
    float DamageScale = 2.0f;

private:
    virtual bool Apply(AActor* ModifiedActor) override;
    virtual void Revert() override;

    ARShip* ModifiedShip = nullptr;
    float PreviousDamageScale = 1.0f;
};
