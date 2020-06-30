#pragma once

#include <GameFramework/PlayerController.h>
#include "RPlayerController.generated.h"

class UInputComponent;

UCLASS()
class REIKONOIDS_API ARPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> CameraAttachmentClass;

protected:
    ARPlayerController();

    void Exit();
    void Proceed();

    virtual void SetupInputComponent() override;
    virtual void OnPossess(APawn* PossesedPawn) override;
    virtual void UpdateRotation(float DeltaTime) override;
    virtual void OnUnPossess() override;
    virtual void BeginDestroy() override;

private:
    UPROPERTY()
    AActor* CameraAttachment = nullptr;
};
