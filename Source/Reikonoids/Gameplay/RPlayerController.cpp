#include "RPlayerController.h"

ARPlayerController::ARPlayerController()
{
    // Set to false otherwise camera will be reset after pawn is destroyed.
    bAutoManageActiveCameraTarget = false;
}

void ARPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("Exit", IE_Pressed, this, &ARPlayerController::Exit);
    InputComponent->BindAction("Proceed", IE_Pressed, this, &ARPlayerController::Proceed);
}

void ARPlayerController::Exit()
{
    FGenericPlatformMisc::RequestExit(false);
}

void ARPlayerController::Proceed()
{
    OnProceedAction.Broadcast();
}

void ARPlayerController::OnPossess(APawn* PossesedPawn)
{
    Super::OnPossess(PossesedPawn);

    if(CameraAttachmentClass && !CameraAttachment)
    {
        // Spawn camera attachment.
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;

        CameraAttachment = GetWorld()->SpawnActor<AActor>(CameraAttachmentClass, SpawnParams);
    }

    if(CameraAttachment)
    {
        // Attach camera to possessed pawn.
        FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, false);
        AttachmentRules.LocationRule = EAttachmentRule::KeepRelative;
        CameraAttachment->AttachToActor(PossesedPawn, AttachmentRules);

        // Use attachment as view target.
        FViewTargetTransitionParams TransitionParams;
        TransitionParams.BlendTime = 0.0f;
        PlayerCameraManager->SetViewTarget(CameraAttachment, TransitionParams);
    }
}

void ARPlayerController::UpdateRotation(float DeltaTime)
{
    // Hack fix for shipping configuration having too quick rotations because of following UE4 issues:
    // - Shipping configuration has framerate cap disabled, while in Debug/Development there is 120FPS limit (why?!).
    // - Having bUseControllerRotationYaw=true results in controller rotating pawn every tick without accounting for delta time.
    RotationInput *= DeltaTime * 120.0f;

    Super::UpdateRotation(DeltaTime);
}

void ARPlayerController::OnUnPossess()
{
    Super::OnUnPossess();
}

void ARPlayerController::BeginDestroy()
{
    // Destroy camera attachment only when controller is destroyed as well.
    // Previously it was done on unpossession but this caused issues when pawn died.
    if(CameraAttachment)
    {
        CameraAttachment->Destroy();
        CameraAttachment = nullptr;
    }

    Super::BeginDestroy();
}
