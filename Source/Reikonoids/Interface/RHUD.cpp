#include "RHUD.h"
#include <Blueprint/UserWidget.h>

ARHUD::ARHUD() = default;
ARHUD::~ARHUD() = default;

void ARHUD::BeginPlay()
{
    Super::BeginPlay();

    // Create HUD widget.
    if(WidgetClass)
    {
        Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
        Widget->AddToViewport();
    }
}
