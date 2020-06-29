#include "RWeaponComponent.h"
#include "../Actors/RProjectile.h"
#include <Kismet/GameplayStatics.h>
#include <Components/SphereComponent.h>

URWeaponComponent::URWeaponComponent()
{
    // Setup default shooting pattern.
    FiringPattern.FireIndexCount = 1;
    FiringPattern.FiringEntries.Add(FRProjectileFiringEntry());
}

void URWeaponComponent::StartFiring()
{
    // Start firing timer and use remaining time of the previous timer as delay.
    // Delay prevents firing too quickly when this is called repeatedly (e.g. when input is spammed).
    FTimerManager& TimerManager = GetWorld()->GetTimerManager();
    float RemainingTime = FMath::Max(TimerManager.GetTimerRemaining(FireTimer), 0.0f);
    TimerManager.SetTimer(FireTimer, this, &URWeaponComponent::FireProjectile, 1.0f / FireRatePerSecond, false, RemainingTime);
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

    for(const auto& ShootingEntry : FiringPattern.FiringEntries)
    {
        // Check if entry is valid for current firing index.
        if(ShootingEntry.FireIndex != FireCount % FiringPattern.FireIndexCount)
            continue;

        // Spawn projectile.
        FActorSpawnParameters SpawnParams;
        SpawnParams.Instigator = Cast<APawn>(GetOwner());
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        SpawnParams.bDeferConstruction = true;

        ARProjectile* Projectile = GetWorld()->SpawnActor<ARProjectile>(ProjectileClass, SpawnParams);

        // Setup projectile.
        Projectile->Damage *= ProjectileDamageScale;
        Projectile->PierceCount = ProjectilePierceCount;

        FTransform SpawnTransform;
        SpawnTransform.SetLocation(ShootingEntry.Location);
        SpawnTransform.SetRotation(ShootingEntry.Rotation.Quaternion());
        SpawnTransform = SpawnTransform * GetComponentToWorld();
 
        UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
    }

    // Broadcast weapon fired event.
    OnWeaponFired.Broadcast();

    // Increment fire index.
    FireCount += 1;

    // Start new timer.
    FTimerManager& TimerManager = GetWorld()->GetTimerManager();
    TimerManager.SetTimer(FireTimer, this, &URWeaponComponent::FireProjectile, 1.0f / FireRatePerSecond, false);
}
