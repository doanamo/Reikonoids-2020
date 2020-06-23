#pragma once

#include <GameFramework/Pawn.h>
#include "RShip.generated.h"

class USphereComponent;
class URHealthComponent;
class URModifierStackComponent;
class URWeaponComponent;

UCLASS()
class REIKONOIDS_API ARShip : public APawn
{
    GENERATED_BODY()

public:
    virtual ~ARShip();

    void StartFiring();
    void StopFiring();
    void MoveForward(float AxisScale);
    void RotateRight(float AxisScale);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MovementImpulseSize = 500.0f;

    UPROPERTY(EditDefaultsOnly)
    float LinearDampingForce = 1.0f;

    UPROPERTY(VisibleAnywhere)
    USphereComponent* SphereCollision = nullptr;

    UPROPERTY(VisibleAnywhere)
    URHealthComponent* HealthComponent = nullptr;

    UPROPERTY(VisibleAnywhere)
    URModifierStackComponent* ModifierStack = nullptr;

    UPROPERTY(VisibleAnywhere)
    URWeaponComponent* WeaponComponent = nullptr;

protected:
    ARShip();

    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void OnDeath();
};
