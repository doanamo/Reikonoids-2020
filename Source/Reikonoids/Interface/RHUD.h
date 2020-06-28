#pragma once

#include <GameFramework/HUD.h>
#include "RHUD.generated.h"

class UUserWidget;

UCLASS()
class REIKONOIDS_API ARHUD : public AHUD
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UUserWidget> WidgetClass;

    UPROPERTY()
    UUserWidget* Widget = nullptr;

    virtual ~ARHUD();

protected:
    ARHUD();

    virtual void BeginPlay() override;
};
