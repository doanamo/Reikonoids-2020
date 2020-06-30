#include "RBehaviorTreeTasks.h"
#include "RAIController.h"
#include "../Actors/RShip.h"
#include <Components/ActorComponent.h>
#include <BehaviorTree/BehaviorTreeComponent.h>

ARShip* GetShipPawnFromComponent(UBehaviorTreeComponent& Component)
{
    ARAIController* Controller = Cast<ARAIController>(Component.GetOwner());
    if(!Controller)
    {
        return nullptr;
    }

    ARShip* Ship = Cast<ARShip>(Controller->GetPawn());
    if(!Ship)
    {
        return nullptr;
    }

    return Ship;
}

EBTNodeResult::Type URBehaviorTreeTask_MoveForward::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    // Move ship forward.
    ARShip* Ship = GetShipPawnFromComponent(OwnerComp);

    if(Ship)
    {
        Ship->MoveForward(MovementScale);
    }

    return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type URBehaviorTreeTask_StartFiring::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    // Start firing weapon.
    ARShip* Ship = GetShipPawnFromComponent(OwnerComp);

    if(Ship)
    {
        Ship->StartFiring();
    }

    return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type URBehaviorTreeTask_StopFiring::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    // Stop firing weapon.
    ARShip* Ship = GetShipPawnFromComponent(OwnerComp);

    if(Ship)
    {
        Ship->StopFiring();
    }

    return EBTNodeResult::Succeeded;
}
