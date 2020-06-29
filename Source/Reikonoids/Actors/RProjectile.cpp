#include "RProjectile.h"
#include <Components/SphereComponent.h>

ARProjectile::ARProjectile()
{
    PrimaryActorTick.bCanEverTick = true;

    // Setup sphere collision component.
    SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
    SphereCollision->SetSphereRadius(16.0f);
    RootComponent = SphereCollision;
}

ARProjectile::~ARProjectile() = default;

void ARProjectile::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    // React when projectile hits another actor.
    AActor::OnActorBeginOverlap.AddDynamic(this, &ARProjectile::OnActorBeginOverlap);
}

void ARProjectile::BeginPlay()
{
    Super::BeginPlay();

    // Expire projectile when life time ends.
    GetWorld()->GetTimerManager().SetTimer(LifetimeTimer, this, &ARProjectile::OnExpire, LifeTime, false);
}

void ARProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Move projectile forward at constant speed with sweep enabled.
    SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime, true);
}

void ARProjectile::OnActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    check(OverlappedActor == this);
    check(OtherActor != nullptr);

    // Check if projectile is already marked for destruction
    // to prevent dealing damage to multiple actors at same time.
    if(IsActorBeingDestroyed())
        return;

    // Do not damage actor who fired this projectile.
    if(GetInstigator() == OtherActor)
        return;

    // Ignore overlap with other projectiles from same instigator.
    if(GetInstigator() == OtherActor->GetInstigator())
    {
        if(ARProjectile* OtherPojectile = Cast<ARProjectile>(OtherActor))
            return;
    }

    // Check if overlapped an ignored actor.
    if(IgnoredActors.Find(OtherActor) != INDEX_NONE)
        return;

    // Make overlapped actor receive damage.
    FDamageEvent DamageEvent;
    OtherActor->TakeDamage(Damage, DamageEvent, nullptr, this);

    // Handle projectile piercing.
    if(PierceCount > 0)
    {
        PierceCount -= 1;

        // Ignore actor pair to prevent repeating overlap event.
        IgnoredActors.Add(OtherActor);
    }
    else
    {
        // Destroy projectile.
        ensure(Destroy());
    }
}

void ARProjectile::OnExpire()
{
    // Destroy when life time ends.
    Destroy();
}
