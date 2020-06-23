#pragma once

#include <GameFramework/Actor.h>
#include "RDrop.generated.h"

class URModifier;
class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class REIKONOIDS_API ARDrop : public AActor
{
    GENERATED_BODY()

public:
    virtual ~ARDrop();

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<URModifier> ModifierClass;

    UPROPERTY(VisibleAnywhere)
    USphereComponent* SphereCollision = nullptr;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* StaticMesh = nullptr;

protected:
    ARDrop();

    virtual void PostInitializeComponents() override;

    UFUNCTION()
    void OnActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
};
