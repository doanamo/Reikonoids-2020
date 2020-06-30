#pragma once

#include <Components/ActorComponent.h>
#include "RModifierStackComponent.generated.h"

class URModifier_Base;

UCLASS()
class REIKONOIDS_API URModifierStackComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    URModifierStackComponent();

    bool ApplyModifier(URModifier_Base* Modifier);

protected:
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY()
    TArray<URModifier_Base*> ModifierStack;
};
