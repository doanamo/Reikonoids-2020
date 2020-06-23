#include "RModifier_ProjectileDamage.h"
#include "../../Actors/RShip.h"
#include "../../Components/RWeaponComponent.h"

URModifier_ProjectileDamage::URModifier_ProjectileDamage()
{
    URModifier_Base::InitialDuration = 10.0f;
}

URModifier_ProjectileDamage::~URModifier_ProjectileDamage() = default;

bool URModifier_ProjectileDamage::Apply(AActor* ModifiedActor)
{
    if(!Super::Apply(ModifiedActor))
        return false;

    // Check for correct actor class and acquire needed component.
    ModifiedShip = Cast<ARShip>(ModifiedActor);
    if(ModifiedShip == nullptr)
        return false;

    URWeaponComponent* WeaponComponent = ModifiedShip->WeaponComponent;
    check(WeaponComponent);

    // Modify projectile damage scale.
    PreviousDamageScale = WeaponComponent->ProjectileDamageScale;
    WeaponComponent->ProjectileDamageScale *= DamageScale;

    return true;
}

void URModifier_ProjectileDamage::Revert()
{
    Super::Revert();

    // Acquire needed component.
    check(ModifiedShip != nullptr);
    URWeaponComponent* WeaponComponent = ModifiedShip->WeaponComponent;
    check(WeaponComponent);

    // Revert projectile damage scale to previous value.
    WeaponComponent->ProjectileDamageScale = PreviousDamageScale;
}
