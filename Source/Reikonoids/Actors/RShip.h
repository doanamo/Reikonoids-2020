#pragma once

#include <GameFramework/Pawn.h>
#include "RShip.generated.h"

class USphereComponent;
class URHealthComponent;
class URWeaponComponent;

UCLASS()
class REIKONOIDS_API ARShip : public APawn
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MovementImpulseSize = 800.0f;

    UPROPERTY(EditDefaultsOnly)
    float LinearDampingForce = 1.0f;

    void StartFiring();
    void StopFiring();
    void MoveForward(float AxisScale);
    void RotateRight(float AxisScale);

protected:
    ARShip();

    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void OnDeath();

protected:
    UPROPERTY(VisibleAnywhere)
    USphereComponent* SphereCollision = nullptr;

    UPROPERTY(VisibleAnywhere)
    URHealthComponent* HealthComponent = nullptr;

    UPROPERTY(VisibleAnywhere)
    URWeaponComponent* WeaponComponent = nullptr;
};
