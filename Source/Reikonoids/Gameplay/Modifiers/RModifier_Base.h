#pragma once

#include <UObject/Object.h>
#include "RModifier_Base.generated.h"

class AActor;

UCLASS(EditInlineNew)
class REIKONOIDS_API URModifier_Base : public UObject
{
    GENERATED_BODY()

public:
    virtual ~URModifier_Base();

    UPROPERTY(EditAnywhere)
    float InitialDuration = 0.0f;

protected:
    friend class URModifierStackComponent;

    URModifier_Base();

    virtual bool Apply(AActor* ModifiedActor);
    virtual void Tick(float DeltaTime);
    virtual void Revert();

    bool IsExpired() const;

    float RemainingDuration = 0.0f;
};
