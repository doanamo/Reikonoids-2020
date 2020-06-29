#pragma once

#include <Components/SceneComponent.h>
#include "RWeaponComponent.generated.h"

class ARProjectile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponFiredSignature);

USTRUCT()
struct REIKONOIDS_API FRProjectileFiringEntry
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    int FireIndex = 0;

    UPROPERTY(EditDefaultsOnly)
    FVector Location = FVector::ZeroVector;

    UPROPERTY(EditDefaultsOnly)
    FRotator Rotation = FRotator::ZeroRotator;
};

USTRUCT()
struct REIKONOIDS_API FRWeaponFiringPattern
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    int FireIndexCount = 0;

    UPROPERTY(EditDefaultsOnly)
    TArray<FRProjectileFiringEntry> FiringEntries;
};

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float FireRatePerSecond = 1.0f;

    UPROPERTY(EditDefaultsOnly)
    FRWeaponFiringPattern FiringPattern;

    UPROPERTY(BlueprintAssignable)
    FOnWeaponFiredSignature OnWeaponFired;

    URWeaponComponent();

    void StartFiring();
    void StopFiring();

private:
    void FireProjectile();

    FTimerHandle FireTimer;
    int FireCount = 0;
};
