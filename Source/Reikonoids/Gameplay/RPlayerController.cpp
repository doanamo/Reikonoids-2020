#include "RPlayerController.h"

ARPlayerController::ARPlayerController()
{
}

void ARPlayerController::OnPossess(APawn* PossesedPawn)
{
    Super::OnPossess(PossesedPawn);

    if(CameraAttachmentClass)
    {
        // Spawn camera attachment.
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;

        FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, false);
        AttachmentRules.LocationRule = EAttachmentRule::KeepRelative;

        CameraAttachment = GetWorld()->SpawnActor<AActor>(CameraAttachmentClass, SpawnParams);
        CameraAttachment->AttachToActor(PossesedPawn, AttachmentRules);

        // Use attachment as view target.
        FViewTargetTransitionParams TransitionParams;
        TransitionParams.BlendTime = 0.0f;

        PlayerCameraManager->SetViewTarget(CameraAttachment, TransitionParams);
    }
}

void ARPlayerController::OnUnPossess()
{
    if(CameraAttachment)
    {
        CameraAttachment->Destroy();
        CameraAttachment = nullptr;
    }

    Super::OnUnPossess();
}
