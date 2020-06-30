#include "RHealthComponent.h"

URHealthComponent::URHealthComponent()
{
    bWantsInitializeComponent = true;
}

void URHealthComponent::InitializeComponent()
{
    Super::InitializeComponent();

    // Sanitize default values.
    MaximumHealth = FMath::Max(1.0f, MaximumHealth);
    CurrentHealth = FMath::Min(CurrentHealth, MaximumHealth);

    if(CurrentHealth < 0.0f)
    {
        CurrentHealth = MaximumHealth;
    }

    // Hook component to game framework's damage event system.
    GetOwner()->OnTakeAnyDamage.AddDynamic(this, &URHealthComponent::OnTakeAnyDamage);
}

void URHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    // Broadcast actor's death event if initial health is zero.
    if(CurrentHealth == 0.0f)
    {
        OnDeath.Broadcast(nullptr);
    }
}

void URHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
    if(IsDead())
        return;

    // Calculate scaled damage.
    float ScaledDamage = Damage;

    if(Damage > 0.0f)
    {
        ScaledDamage *= DamageScale;
    }

    if(FMath::IsNearlyZero(ScaledDamage))
        return;

    // Print debug damage info.
#ifdef WITH_EDITOR
    if(GEngine)
    {
        if(ScaledDamage > 0.0f)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow,
                FString::Printf(TEXT("%s received %f damage from %s"),
                *GetOwner()->GetName(), ScaledDamage, *DamageCauser->GetName()));
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow,
                FString::Printf(TEXT("%s received %f healing from %s"),
                *GetOwner()->GetName(), -ScaledDamage, *DamageCauser->GetName()));
        }
    }
#endif

    // Update current health value.
    float UpdatedHealth = FMath::Clamp(CurrentHealth - ScaledDamage, 0.0f, MaximumHealth);

    if(UpdatedHealth != CurrentHealth)
    {
        OnHealthChange.Broadcast(UpdatedHealth, MaximumHealth);
    }

    CurrentHealth = UpdatedHealth;

    // Broadcast actor's death event on fatal damage.
    if(CurrentHealth <= 0.0f)
    {
        OnDeath.Broadcast(InstigatedBy);
    }
}

bool URHealthComponent::IsDead() const
{
    return CurrentHealth <= 0.0f;
}
