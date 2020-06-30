#pragma once

#include <AIController.h>
#include "RAIController.generated.h"

class UBehaviorTree;
class UPawnSensingComponent;

UCLASS()
class REIKONOIDS_API ARAIController : public AAIController
{
    GENERATED_BODY()

public:
    virtual ~ARAIController();

    UPROPERTY(EditDefaultsOnly)
    UBehaviorTree* BehaviorTree = nullptr;

    UPROPERTY(VisibleAnywhere)
    UPawnSensingComponent* PawnSensingComponent = nullptr;

protected:
    ARAIController();

    virtual void OnPossess(APawn* PossesedPawn) override;
    virtual void Tick(float DeltaTime) override;
    virtual void OnUnPossess() override;

    UFUNCTION()
    void OnSeePawn(APawn* SeenPawn);

private:

};
