#pragma once

#include <BehaviorTree/Tasks/BTTask_BlackboardBase.h>
#include "RBehaviorTreeTasks.generated.h"

UCLASS()
class REIKONOIDS_API URBehaviorTreeTask_MoveForward : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    float MovementScale = 1.0f;

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

UCLASS()
class REIKONOIDS_API URBehaviorTreeTask_StartFiring : public UBTTaskNode
{
    GENERATED_BODY()

public:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

UCLASS()
class REIKONOIDS_API URBehaviorTreeTask_StopFiring : public UBTTaskNode
{
    GENERATED_BODY()

public:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
