#pragma once

#include <GameFramework/Character.h>
#include "RShip.generated.h"

class URHealthComponent;
class URWeaponComponent;

UCLASS()
class REIKONOIDS_API ARShip : public ACharacter
{
    GENERATED_BODY()

public:
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
    URHealthComponent* HealthComponent = nullptr;

    UPROPERTY(VisibleAnywhere)
    URWeaponComponent* WeaponComponent = nullptr;
};
