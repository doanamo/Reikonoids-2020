#pragma once

#include <GameFramework/Actor.h>
#include "RProjectile.generated.h"

class USphereComponent;

UCLASS()
class REIKONOIDS_API ARProjectile : public AActor
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly)
    float Damage = 10.0f;

    UPROPERTY(EditDefaultsOnly)
    float LifeTime = 3.0f;

    UPROPERTY(EditDefaultsOnly)
    float Speed = 1000.0f;

protected:
    ARProjectile();

    virtual void PostInitializeComponents() override;
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void OnActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

    UFUNCTION()
    void OnExpire();

private:
    FTimerHandle LifetimeTimer;

    UPROPERTY(VisibleAnywhere)
    USphereComponent* SphereCollision = nullptr;
};
