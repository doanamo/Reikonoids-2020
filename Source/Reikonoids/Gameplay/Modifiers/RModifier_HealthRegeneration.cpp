#include "RModifier_HealthRegeneration.h"
#include "../../Actors/RShip.h"

URModifier_HealthRegeneration::URModifier_HealthRegeneration()
{
    URModifier_Base::RemainingDuration = 10.0f;
}

URModifier_HealthRegeneration::~URModifier_HealthRegeneration() = default;

bool URModifier_HealthRegeneration::Apply(AActor* ModifiedActor)
{
    if(!Super::Apply(ModifiedActor))
        return false;

    // Check for correct actor class.
    ModifiedShip = Cast<ARShip>(ModifiedActor);
    if(ModifiedShip == nullptr)
        return false;

    return true;
}

void URModifier_HealthRegeneration::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    check(ModifiedShip != nullptr);

    // Accumulate time to only apply healing every second.
    SecondAccumulator += DeltaSeconds;
    while(SecondAccumulator >= 1.0f)
    {
        SecondAccumulator -= 1.0f;

        // Heal target ship by using negative damage.
        FDamageEvent DamageEvent;
        ModifiedShip->TakeDamage(-1.0f * HealthRegenerationPerSecond, DamageEvent, nullptr, ModifiedShip);
    }
}
