#pragma once

#include <Components/SceneComponent.h>
#include "RWeaponComponent.generated.h"

class ARProjectile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponFiredSignature);

UCLASS()
class REIKONOIDS_API URWeaponComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<ARProjectile> ProjectileClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FireRatePerSecond = 1.0f;

    UPROPERTY(BlueprintAssignable)
    FOnWeaponFiredSignature OnWeaponFired;

public:
    URWeaponComponent();

    void StartFiring();
    void StopFiring();

private:
    void FireProjectile();

private:
    FTimerHandle FireTimer;
};
