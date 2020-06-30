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

    UFUNCTION(BlueprintCallable)
    void StartFiring();

    UFUNCTION(BlueprintCallable)
    void StopFiring();

    UFUNCTION(BlueprintCallable)
    void MoveForward(float AxisScale);

    UFUNCTION(BlueprintCallable)
    void RotateRight(float AxisScale);

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float MovementImpulseSize = 50000.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float LinearDampingForce = 1.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USphereComponent* SphereCollision = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    URHealthComponent* HealthComponent = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    URModifierStackComponent* ModifierStack = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    URWeaponComponent* WeaponComponent = nullptr;

protected:
    ARShip();

    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    virtual void FaceRotation(FRotator NewControlRotation, float DeltaTime) override;

    UFUNCTION()
    void OnDeath();

    float ForwardMovementInput = 0.0f;
};
