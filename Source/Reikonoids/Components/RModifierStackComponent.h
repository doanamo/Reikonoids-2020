#pragma once

#include <Components/ActorComponent.h>
#include "RModifierStackComponent.generated.h"

class URModifier;

UCLASS()
class REIKONOIDS_API URModifierStackComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    URModifierStackComponent();

    bool ApplyModifier(URModifier* Modifier);

private:
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    TArray<URModifier*> ModifierStack;
};
