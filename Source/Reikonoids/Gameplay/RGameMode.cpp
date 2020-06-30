#include "RGameMode.h"
#include "RSpawnDirector.h"
#include <Kismet/GameplayStatics.h>

ARGameMode::ARGameMode()
{
    SpawnDirector = CreateDefaultSubobject<URSpawnDirector>(TEXT("SpawnDirector"));
}

ARGameMode::~ARGameMode() = default;

void ARGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Setup spawn director population update.
    SpawnDirector->SetupPopulationUpdate(GetWorld());
    SpawnDirector->OverrideMinSpawnRadiusOnNextUpdate(600.0f);
}

void ARGameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    // Move spawn origin.
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if(APawn* Pawn = PlayerController->GetPawn())
    {
        SpawnDirector->SetSpawnOrigin(Pawn->GetActorLocation());
    }
}
