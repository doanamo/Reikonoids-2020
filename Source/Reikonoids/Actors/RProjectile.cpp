#include "RProjectile.h"

ARProjectile::ARProjectile()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ARProjectile::BeginPlay()
{
    Super::BeginPlay();
}

void ARProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Update life time.
    LifeTime = std::max(0.0f, LifeTime - DeltaTime);
    if(LifeTime == 0.0f)
    {
        Destroy();
    }

    // Orientate projectile towards direction.
    SetActorRotation(Direction.ToOrientationRotator());

    // Move projectile forward at constant speed.
    SetActorLocation(GetActorLocation() + Direction * Speed * DeltaTime);
}
