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

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float ProjectileDamageScale = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int ProjectilePierceCount = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FireRatePerSecond = 1.0f;

    UPROPERTY(BlueprintAssignable)
    FOnWeaponFiredSignature OnWeaponFired;

    URWeaponComponent();

    void StartFiring();
    void StopFiring();

private:
    void FireProjectile();

    FTimerHandle FireTimer;
};
