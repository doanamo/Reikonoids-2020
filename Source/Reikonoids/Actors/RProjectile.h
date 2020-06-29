#pragma once

#include <GameFramework/Actor.h>
#include "RProjectile.generated.h"

class USphereComponent;

UCLASS()
class REIKONOIDS_API ARProjectile : public AActor
{
    GENERATED_BODY()

public:
    virtual ~ARProjectile();

    UPROPERTY(EditDefaultsOnly)
    float Damage = 10.0f;

    UPROPERTY(EditDefaultsOnly)
    int PierceCount = 0;

    UPROPERTY(EditDefaultsOnly)
    float LifeTime = 3.0f;

    UPROPERTY(EditDefaultsOnly)
    float Speed = 1000.0f;

    UPROPERTY(VisibleAnywhere)
    USphereComponent* SphereCollision = nullptr;

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

    UPROPERTY()
    TArray<AActor*> IgnoredActors;
};
