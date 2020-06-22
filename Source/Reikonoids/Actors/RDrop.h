#pragma once

#include <GameFramework/Actor.h>
#include "RDrop.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class REIKONOIDS_API ARDrop : public AActor
{
    GENERATED_BODY()

protected:
    ARDrop();

private:
    UPROPERTY(VisibleAnywhere)
    USphereComponent* SphereCollision = nullptr;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* StaticMesh = nullptr;
};
