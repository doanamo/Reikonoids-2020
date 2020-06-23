#include "RModifier_FireRate.h"
#include "../../Actors/RShip.h"
#include "../../Components/RWeaponComponent.h"

URModifier_FireRate::URModifier_FireRate()
{
    URModifier::RemainingDuration = 10.0f;
}

URModifier_FireRate::~URModifier_FireRate() = default;

bool URModifier_FireRate::Apply(AActor* ModifiedActor)
{
    if(!Super::Apply(ModifiedActor))
        return false;

    // Check for correct actor class.
    ModifiedShip = Cast<ARShip>(ModifiedActor);
    if(ModifiedShip == nullptr)
        return false;

    // Acquire weapon component and save its current fire rate.
    URWeaponComponent* WeaponComponent = ModifiedShip->WeaponComponent;
    check(WeaponComponent);

    PreviousFireRate = WeaponComponent->FireRatePerSecond;

    // Modify weapon fire rate.
    WeaponComponent->FireRatePerSecond *= FireRateScale;

    return true;
}

void URModifier_FireRate::Revert()
{
    Super::Revert();

    check(ModifiedShip != nullptr);

    // Revert fire rate to previous value.
    URWeaponComponent* WeaponComponent = ModifiedShip->WeaponComponent;
    check(WeaponComponent);

    WeaponComponent->FireRatePerSecond = PreviousFireRate;
}
