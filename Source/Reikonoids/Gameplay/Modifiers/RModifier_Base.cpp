#include "RModifier_Base.h"

URModifier_Base::URModifier_Base() = default;
URModifier_Base::~URModifier_Base() = default;

bool URModifier_Base::Apply(AActor* ModifiedActor)
{
    return !IsExpired();
}

void URModifier_Base::Tick(float DeltaTime)
{
    // Update remaining modifier duration, causing it to eventually expire.
    RemainingDuration = FMath::Max(0.0f, RemainingDuration - DeltaTime);
}

void URModifier_Base::Revert()
{
}

bool URModifier_Base::IsExpired() const
{
    return RemainingDuration == 0.0f;
}
