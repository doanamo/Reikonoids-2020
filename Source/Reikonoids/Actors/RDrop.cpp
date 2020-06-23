#include "RDrop.h"
#include "RShip.h"
#include "../Gameplay/RModifier.h"
#include "../Components/RModifierStackComponent.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>

ARDrop::ARDrop()
{
    // Setup sphere collision component.
    SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    SphereCollision->SetSimulatePhysics(true);
    SphereCollision->SetSphereRadius(32.0f);
    SphereCollision->SetCollisionProfileName(FName("OverlapOnlyPawn"));
    SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    SphereCollision->BodyInstance.bLockZTranslation = true;
    SphereCollision->BodyInstance.bLockXRotation = true;
    SphereCollision->BodyInstance.bLockYRotation = true;
    RootComponent = SphereCollision;

    // Setup mesh component.
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetCollisionProfileName(FName("NoCollision"));
    StaticMesh->SetupAttachment(SphereCollision);
}

ARDrop::~ARDrop() = default;

void ARDrop::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    // React when drop overlaps another actor.
    AActor::OnActorBeginOverlap.AddDynamic(this, &ARDrop::OnActorBeginOverlap);
}

void ARDrop::OnActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    check(OverlappedActor == this);
    check(OtherActor != nullptr);

    // Check if drop is already marked for destruction to prevent
    // granting modifiers to multiple actors at same time.
    if(IsActorBeingDestroyed())
        return;

    // Check if overlapping valid actor archetype.
    if(ARShip* Ship = Cast<ARShip>(OtherActor))
    {
        // Acquire modifier stack component.
        URModifierStackComponent* ModifierStack = Ship->ModifierStack;
        check(ModifierStack != nullptr);

        // Create and apply modifier.
        if(ModifierClass)
        {
            URModifier* Modifier = NewObject<URModifier>(this, ModifierClass);
            if(!ModifierStack->ApplyModifier(Modifier))
                return;
        }

        // Destroy drop.
        ensure(Destroy());
    }
}
