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
    virtual ~ARAsteroid();

    void ApplyRandomTorque();
    void ApplyRandomVelocity();

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

    UPROPERTY(VisibleAnywhere)
    USphereComponent* SphereCollision = nullptr;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* StaticMesh = nullptr;

    UPROPERTY(VisibleAnywhere)
    URHealthComponent* HealthComponent = nullptr;

protected:
    ARAsteroid();

    virtual void BeginPlay() override;

    UFUNCTION()
    void OnDeath();
};
