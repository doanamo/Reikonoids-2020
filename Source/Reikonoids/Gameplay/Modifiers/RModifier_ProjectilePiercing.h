#pragma once

#include "RModifier_Base.h"
#include "RModifier_ProjectilePiercing.generated.h"

class ARShip;

UCLASS()
class REIKONOIDS_API URModifier_ProjectilePiercing : public URModifier_Base
{
    GENERATED_BODY()

public:
    URModifier_ProjectilePiercing();
    ~URModifier_ProjectilePiercing();

    UPROPERTY(EditDefaultsOnly)
    int PierceCount = 8;

private:
    virtual bool Apply(AActor* ModifiedActor) override;
    virtual void Revert() override;

    ARShip* ModifiedShip = nullptr;
    int PreviousPierceCount = 0;
};
