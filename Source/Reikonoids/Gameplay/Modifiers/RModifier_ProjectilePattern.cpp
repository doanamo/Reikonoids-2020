#include "RModifier_ProjectilePattern.h"
#include "../../Actors/RShip.h"

URModifier_ProjectilePattern::URModifier_ProjectilePattern()
{
    URModifier_Base::Name = NSLOCTEXT("Modifier", "ProjectilePattern", "Projectile Pattern");
    URModifier_Base::InitialDuration = 10.0f;
}

URModifier_ProjectilePattern::~URModifier_ProjectilePattern() = default;

bool URModifier_ProjectilePattern::Apply(AActor* ModifiedActor)
{
    if(!Super::Apply(ModifiedActor))
        return false;

    // Check for correct actor class and acquire needed component.
    ModifiedShip = Cast<ARShip>(ModifiedActor);
    if(ModifiedShip == nullptr)
        return false;

    URWeaponComponent* WeaponComponent = ModifiedShip->WeaponComponent;
    check(WeaponComponent);

    // Modify projectile firing pattern.
    PreviousFiringPattern = WeaponComponent->FiringPattern;
    WeaponComponent->FiringPattern = FiringPattern;

    return true;
}

void URModifier_ProjectilePattern::Revert()
{
    Super::Revert();

    // Acquire needed component.
    check(ModifiedShip != nullptr);
    URWeaponComponent* WeaponComponent = ModifiedShip->WeaponComponent;
    check(WeaponComponent);

    // Revert projectile firing pattern.
    WeaponComponent->FiringPattern = PreviousFiringPattern;
}
