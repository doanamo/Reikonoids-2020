#include "RModifier_MovementSpeed.h"
#include "../../Actors/RShip.h"

URModifier_MovementSpeed::URModifier_MovementSpeed()
{
    URModifier_Base::Name = NSLOCTEXT("Modifier", "MovementSpeed", "Movement Speed");
    URModifier_Base::InitialDuration = 10.0f;
}

URModifier_MovementSpeed::~URModifier_MovementSpeed() = default;

bool URModifier_MovementSpeed::Apply(AActor* ModifiedActor)
{
    if(!Super::Apply(ModifiedActor))
        return false;

    // Check for correct actor class.
    ModifiedShip = Cast<ARShip>(ModifiedActor);
    if(ModifiedShip == nullptr)
        return false;

    // Modify movement impulse size.
    PreviousMovementImpulseSize = ModifiedShip->MovementImpulseSize;
    ModifiedShip->MovementImpulseSize *= MovementImpulseSizeScale;

    return true;
}

void URModifier_MovementSpeed::Revert()
{
    Super::Revert();

    check(ModifiedShip != nullptr);

    // Revert movement impulse size.
    ModifiedShip->MovementImpulseSize = PreviousMovementImpulseSize;
}
