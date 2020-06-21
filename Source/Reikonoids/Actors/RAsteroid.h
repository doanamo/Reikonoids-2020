#pragma once

#include <GameFramework/Actor.h>
#include "RAsteroid.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class URHealthComponent;

UCLASS()
class REIKONOIDS_API ARAsteroid : public AActor
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly)
    int FractureIndex = 3;

    UPROPERTY(EditDefaultsOnly)
    int FractureCount = 2;

    UPROPERTY(EditDefaultsOnly)
    float FractureScale = 0.5f;

    UPROPERTY(EditDefaultsOnly)
    FVector MaxRandomTorque = FVector(100.0f, 100.0f, 100.0f);

    UPROPERTY(EditDefaultsOnly)
    FVector MaxRandomVelocity = FVector(100.0f, 100.0f, 100.0f);

    void ApplyRandomTorque();
    void ApplyRandomVelocity();

protected:
    ARAsteroid();

    virtual void BeginPlay() override;

    UFUNCTION()
    void OnDeath();

private:
    UPROPERTY(VisibleAnywhere)
    USphereComponent* SphereCollision = nullptr;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* StaticMesh = nullptr;

    UPROPERTY(VisibleAnywhere)
    URHealthComponent* HealthComponent = nullptr;
};
