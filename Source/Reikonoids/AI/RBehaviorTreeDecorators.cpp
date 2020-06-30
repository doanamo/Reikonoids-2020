#include "RBehaviorTreeDecorators.h"
#include "RAIController.h"
#include "../Actors/RShip.h"
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BehaviorTree/BlackboardComponent.h>

extern ARShip* GetShipPawnFromComponent(UBehaviorTreeComponent& Component);

void URBehaviorTreeDecorator_DistanceCheck::InitializeFromAsset(UBehaviorTree& Asset)
{
    Super::InitializeFromAsset(Asset);

    UBlackboardData* BBAsset = GetBlackboardAsset();
    if(ensure(BBAsset))
    {
        TargetActor.ResolveSelectedKey(*BBAsset);
    }
}

bool URBehaviorTreeDecorator_DistanceCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

    // Retrieve target actor from blackboard.
    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

    AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject(TargetActor.SelectedKeyName));
    if(!Target)
    {
        return false;
    }

    // Check distance between ship and target.
    ARShip* Ship = GetShipPawnFromComponent(OwnerComp);
    if(!Ship)
    {
        return false;
    }

    return Ship->GetDistanceTo(Target) <= Distance;
}
