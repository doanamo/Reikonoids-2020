#pragma once

#include "RModifier_Base.h"
#include "RModifier_Speed.generated.h"

class ARShip;

UCLASS()
class REIKONOIDS_API URModifier_Speed : public URModifier_Base
{
    GENERATED_BODY()

public:
    URModifier_Speed();
    ~URModifier_Speed();

    UPROPERTY(EditDefaultsOnly)
    float MovementImpulseSizeScale = 2.0f;

private:
    virtual bool Apply(AActor* ModifiedActor) override;
    virtual void Revert() override;

    ARShip* ModifiedShip = nullptr;
    float PreviousMovementImpulseSize = 0.0f;
};
