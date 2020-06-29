#include "RModifier_ProjectilePiercing.h"
#include "../../Actors/RShip.h"
#include "../../Components/RWeaponComponent.h"

URModifier_ProjectilePiercing::URModifier_ProjectilePiercing()
{
    URModifier_Base::Name = NSLOCTEXT("Modifier", "ProjectilePiercing", "Projectile Piercing");
    URModifier_Base::InitialDuration = 10.0f;
}

URModifier_ProjectilePiercing::~URModifier_ProjectilePiercing() = default;

bool URModifier_ProjectilePiercing::Apply(AActor* ModifiedActor)
{
    if(!Super::Apply(ModifiedActor))
        return false;

    // Check for correct actor class and acquire needed component.
    ModifiedShip = Cast<ARShip>(ModifiedActor);
    if(ModifiedShip == nullptr)
        return false;

    URWeaponComponent* WeaponComponent = ModifiedShip->WeaponComponent;
    check(WeaponComponent);

    // Modify projectile pierce count.
    PreviousPierceCount = WeaponComponent->ProjectilePierceCount;
    WeaponComponent->ProjectilePierceCount = PierceCount;

    return true;
}

void URModifier_ProjectilePiercing::Revert()
{
    Super::Revert();

    // Acquire needed component.
    check(ModifiedShip != nullptr);
    URWeaponComponent* WeaponComponent = ModifiedShip->WeaponComponent;
    check(WeaponComponent);

    // Revert projectile pierce count.
    WeaponComponent->ProjectilePierceCount = PreviousPierceCount;
}
