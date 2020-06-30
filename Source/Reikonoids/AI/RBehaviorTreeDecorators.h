#pragma once

#include <BehaviorTree/BTDecorator.h>
#include "RBehaviorTreeDecorators.generated.h"

UCLASS()
class REIKONOIDS_API URBehaviorTreeDecorator_DistanceCheck : public UBTDecorator
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    float Distance = 1000.0f;

    UPROPERTY(EditAnywhere)
    FBlackboardKeySelector TargetActor;

    virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
