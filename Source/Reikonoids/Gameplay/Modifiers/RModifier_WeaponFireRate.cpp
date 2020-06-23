#include "RModifier_WeaponFireRate.h"
#include "../../Actors/RShip.h"
#include "../../Components/RWeaponComponent.h"

URModifier_WeaponFireRate::URModifier_WeaponFireRate()
{
    URModifier_Base::InitialDuration = 10.0f;
}

URModifier_WeaponFireRate::~URModifier_WeaponFireRate() = default;

bool URModifier_WeaponFireRate::Apply(AActor* ModifiedActor)
{
    if(!Super::Apply(ModifiedActor))
        return false;

    // Check for correct actor class and acquire needed component.
    ModifiedShip = Cast<ARShip>(ModifiedActor);
    if(ModifiedShip == nullptr)
        return false;

    URWeaponComponent* WeaponComponent = ModifiedShip->WeaponComponent;
    check(WeaponComponent);

    // Modify weapon fire rate.
    PreviousFireRate = WeaponComponent->FireRatePerSecond;
    WeaponComponent->FireRatePerSecond *= FireRateScale;

    return true;
}

void URModifier_WeaponFireRate::Revert()
{
    Super::Revert();

    // Acquire needed component.
    check(ModifiedShip != nullptr);
    URWeaponComponent* WeaponComponent = ModifiedShip->WeaponComponent;
    check(WeaponComponent);

    // Revert fire rate to previous value.
    WeaponComponent->FireRatePerSecond = PreviousFireRate;
}
