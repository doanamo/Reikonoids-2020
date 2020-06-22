#include "RDrop.h"
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
