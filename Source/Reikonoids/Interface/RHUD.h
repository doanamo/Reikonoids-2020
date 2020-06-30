#pragma once

#include <GameFramework/HUD.h>
#include "RHUD.generated.h"

class UUserWidget;

UCLASS()
class REIKONOIDS_API ARHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual ~ARHUD();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UUserWidget> WidgetClass;

    UPROPERTY()
    UUserWidget* Widget = nullptr;

protected:
    ARHUD();

    virtual void BeginPlay() override;
};
