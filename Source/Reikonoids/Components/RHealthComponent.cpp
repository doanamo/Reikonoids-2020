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

    // Hook component to game framework's delegate called when actor receives damage.
    GetOwner()->OnTakeAnyDamage.AddDynamic(this, &URHealthComponent::OnTakeAnyDamage);
}

void URHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    // Broadcast actor's death event if initial health is zero.
    if(CurrentHealth == 0.0f)
    {
        OnDeath.Broadcast();
    }
}

void URHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
    // Check if already dead.
    if(IsDead())
        return;

    // Print debug damage info.
    if(GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("%s received %f damage from %s"),
            *GetOwner()->GetHumanReadableName(), Damage, *DamageCauser->GetHumanReadableName()));
    }

    // Update current health value.
    CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaximumHealth);

    // Broadcast actor's death event on fatal damage.
    if(CurrentHealth <= 0.0f)
    {
        OnDeath.Broadcast();
    }
}

bool URHealthComponent::IsDead() const
{
    return CurrentHealth <= 0.0f;
}
