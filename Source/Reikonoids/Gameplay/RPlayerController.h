#pragma once

#include <GameFramework/PlayerController.h>
#include "RPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProceedActionSignature);

class UInputComponent;

UCLASS()
class REIKONOIDS_API ARPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> CameraAttachmentClass;

    UPROPERTY(BlueprintAssignable)
    FOnProceedActionSignature OnProceedAction;

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
