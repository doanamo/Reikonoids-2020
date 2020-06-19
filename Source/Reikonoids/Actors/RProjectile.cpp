#include "RProjectile.h"
#include <Components/SphereComponent.h>

ARProjectile::ARProjectile()
{
    PrimaryActorTick.bCanEverTick = true;

    // Setup sphere collision component.
    SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
    SphereCollision->SetSphereRadius(16.0f);
}

void ARProjectile::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    // React when projectile hits another actor.
    AActor::OnActorHit.AddDynamic(this, &ARProjectile::OnActorHit);
}

void ARProjectile::BeginPlay()
{
    Super::BeginPlay();

    // Expire projectile when life time ends.
    GetWorld()->GetTimerManager().SetTimer(LifetimeTimer, this, &ARProjectile::OnExpire, LifeTime, false);

    // Orientate projectile towards direction.
    SetActorRotation(Direction.ToOrientationRotator());
}

void ARProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Move projectile forward at constant speed with sweep enabled.
    SetActorLocation(GetActorLocation() + Direction * Speed * DeltaTime, true);
}

void ARProjectile::OnActorHit(AActor* Self, AActor* Other, FVector NormalImpulse, const FHitResult& Hit)
{
    // Make hit actor receive damage.
    if(Other != nullptr)
    {
        FDamageEvent DamageEvent;
        Other->TakeDamage(Damage, DamageEvent, nullptr, this);
    }

    // Destroy projectile.
    Destroy();
}

void ARProjectile::OnExpire()
{
    // Destroy when life time ends.
    Destroy();
}
