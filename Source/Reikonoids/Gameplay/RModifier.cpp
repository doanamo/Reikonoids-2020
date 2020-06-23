#include "RModifier.h"

URModifier::URModifier() = default;
URModifier::~URModifier() = default;

bool URModifier::Apply(AActor* ModifiedActor)
{
    return !IsExpired();
}

void URModifier::Tick(float DeltaTime)
{
    // Update remaining modifier duration, causing it to eventually expire.
    RemainingDuration = FMath::Max(0.0f, RemainingDuration - DeltaTime);
}

void URModifier::Revert()
{
}

bool URModifier::IsExpired() const
{
    return RemainingDuration == 0.0f;
}
