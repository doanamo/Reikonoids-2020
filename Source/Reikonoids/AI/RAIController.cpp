#include "RAIController.h"
#include <Perception/PawnSensingComponent.h>
#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BehaviorTree/BlackboardComponent.h>

namespace
{
    static const FName CombatTargetName = "CombatTarget";
}

ARAIController::ARAIController()
    : Super()
{
    bAttachToPawn = true;

    // Create pawn sensing component.
    PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
    PawnSensingComponent->SetPeripheralVisionAngle(180.0f);
    PawnSensingComponent->SightRadius = 1400.0f;
}

ARAIController::~ARAIController() = default;

void ARAIController::OnPossess(APawn* PossesedPawn)
{
    Super::OnPossess(PossesedPawn);

    // Delegate has to be set after construction, otherwise it seems to be reset.
    PawnSensingComponent->OnSeePawn.AddDynamic(this, &ARAIController::OnSeePawn);

    // Start behavior tree.
    RunBehaviorTree(BehaviorTree);
}

void ARAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ARAIController::OnUnPossess()
{
    Super::OnUnPossess();
}

void ARAIController::OnSeePawn(APawn* SeenPawn)
{
    if(UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
    {
        BlackboardComponent->SetValueAsObject(CombatTargetName, SeenPawn);
    }
}
