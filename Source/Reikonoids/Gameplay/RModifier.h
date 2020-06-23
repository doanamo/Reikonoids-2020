#pragma once

#include <UObject/Object.h>
#include "RModifier.generated.h"

class AActor;

UCLASS()
class REIKONOIDS_API URModifier : public UObject
{
    GENERATED_BODY()

public:
    virtual ~URModifier();

    UPROPERTY(EditAnywhere)
    float RemainingDuration = 0.0f;

protected:
    friend class URModifierStackComponent;

    URModifier();

    virtual bool Apply(AActor* ModifiedActor);
    virtual void Tick(float DeltaTime);
    virtual void Revert();

    bool IsExpired() const;
};
