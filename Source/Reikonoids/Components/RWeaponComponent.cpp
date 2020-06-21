#include "RWeaponComponent.h"
#include "../Actors/RProjectile.h"

URWeaponComponent::URWeaponComponent() = default;

void URWeaponComponent::StartFiring()
{
    // Start firing timer and use remaining time of the previous timer as delay.
    // Delay prevents firing too quickly when this is called repeatedly (e.g. when input is spammed).
    FTimerManager& TimerManager = GetWorld()->GetTimerManager();
    float RemainingTime = FMath::Max(TimerManager.GetTimerRemaining(FireTimer), 0.0f);
    TimerManager.SetTimer(FireTimer, this, &URWeaponComponent::FireProjectile, 1.0f / FireRatePerSecond, true, RemainingTime);
}

void URWeaponComponent::StopFiring()
{
    // Set timer without delegate that will only indicate remaining fire delay.
    FTimerManager& TimerManager = GetWorld()->GetTimerManager();
    if(TimerManager.TimerExists(FireTimer))
    {
        float RemainingTime = TimerManager.GetTimerRemaining(FireTimer);
        TimerManager.SetTimer(FireTimer, RemainingTime, false);
    }
}

void URWeaponComponent::FireProjectile()
{
    check(ProjectileClass != nullptr);

    // Spawn and setup projectile.
    FActorSpawnParameters SpawnParams;
    SpawnParams.Instigator = Cast<APawn>(GetOwner());
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    ARProjectile* Projectile = GetWorld()->SpawnActor<ARProjectile>(
        ProjectileClass, GetComponentLocation(), GetComponentRotation(), SpawnParams);

    if(Projectile)
    {
        // SpawnActor() may return nullpr if projectile overlaps and deals damage on spawn,
        // leading to actor's instant destruction and nullptr in result. Unreal is weird.
        Projectile->Direction = GetForwardVector();
    }

    // Broadcast weapon fired event.
    OnWeaponFired.Broadcast();
}
