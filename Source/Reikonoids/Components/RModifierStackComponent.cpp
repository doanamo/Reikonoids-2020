#include "RModifierStackComponent.h"
#include "../Gameplay/RModifier.h"

URModifierStackComponent::URModifierStackComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void URModifierStackComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    // Tick all modifiers on stack.
    for(URModifier* Modifier : ModifierStack)
    {
        Modifier->Tick(DeltaTime);
    }

    // Discard modifiers that have expired.
    TArray<URModifier*> RevertedModifers;

    for(int DiscardIndex = ModifierStack.Num() - 1; DiscardIndex >= 0; --DiscardIndex)
    {
        if(ModifierStack[DiscardIndex]->IsExpired())
        {
            // Revert modifiers on stack above one that will be discarded.
            for(int RevertIndex = ModifierStack.Num() - 1; RevertIndex > DiscardIndex; --RevertIndex)
            {
                URModifier* RevertedModifier = ModifierStack.Pop();
                RevertedModifier->Revert();
                RevertedModifers.Add(RevertedModifier);
            }

            // Revert and discard modifier specified by index.
            URModifier* DiscardedModifier = ModifierStack.Pop();
            DiscardedModifier->Revert();

            // Print debug modifier info.
            if(GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("%s lost %s modifier"),
                    *GetOwner()->GetName(), *DiscardedModifier->GetName()));
            }
        }
    }

    // Apply reverted modifiers again.
    for(int ApplyIndex = RevertedModifers.Num() - 1; ApplyIndex >= 0; --ApplyIndex)
    {
        URModifier* RevertedModifier = RevertedModifers.Pop();
        check(RevertedModifier->Apply(GetOwner()));
        ModifierStack.Add(RevertedModifier);
    }
}

bool URModifierStackComponent::ApplyModifier(URModifier* Modifier)
{
    check(Modifier != nullptr);

    // Try to apply modifier.
    if(!Modifier->Apply(GetOwner()))
        return false;

    // Add modifier to the stack.
    ModifierStack.Add(Modifier);

    // Print debug modifier info.
    if(GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("%s gained %s modifier"),
            *GetOwner()->GetName(), *Modifier->GetName()));
    }

    return true;
}
