#pragma once

#include <Components/ActorComponent.h>
#include "RHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangeSignature, float, CurrentHealth, float, MaximumHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);

UCLASS()
class REIKONOIDS_API URHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    static constexpr float FullHealth = -1.0f;

public:
    URHealthComponent();

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
    float MaximumHealth = 100.0f;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
    float CurrentHealth = FullHealth;

    UPROPERTY(BlueprintAssignable)
    FOnHealthChangeSignature OnHealthChange;

    UPROPERTY(BlueprintAssignable)
    FOnDeathSignature OnDeath;

    UFUNCTION()
    bool IsDead() const;

protected:
    virtual void InitializeComponent() override;
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
