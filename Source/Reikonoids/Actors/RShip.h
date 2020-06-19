#pragma once

#include <GameFramework/Character.h>
#include "RShip.generated.h"

class ARProjectile;
class URHealthComponent;

UCLASS()
class REIKONOIDS_API ARShip : public ACharacter
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<ARProjectile> ProjectileClass;

    void Fire();
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
};
