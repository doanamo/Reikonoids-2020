#include "RModifier_Speed.h"
#include "../../Actors/RShip.h"

URModifier_Speed::URModifier_Speed()
{
    URModifier_Base::InitialDuration = 10.0f;
}

URModifier_Speed::~URModifier_Speed() = default;

bool URModifier_Speed::Apply(AActor* ModifiedActor)
{
    if(!Super::Apply(ModifiedActor))
        return false;

    // Check for correct actor class.
    ModifiedShip = Cast<ARShip>(ModifiedActor);
    if(ModifiedShip == nullptr)
        return false;

    // Save previous movement impulse size.
    PreviousMovementImpulseSize = ModifiedShip->MovementImpulseSize;

    // Modify movement impulse size.
    ModifiedShip->MovementImpulseSize *= MovementImpulseSizeScale;

    return true;
}

void URModifier_Speed::Revert()
{
    Super::Revert();

    check(ModifiedShip != nullptr);

    // Revert movement impulse size.
    ModifiedShip->MovementImpulseSize = PreviousMovementImpulseSize;
}
