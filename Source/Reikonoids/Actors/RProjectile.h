#pragma once

#include <GameFramework/Actor.h>
#include "RProjectile.generated.h"

UCLASS()
class REIKONOIDS_API ARProjectile : public AActor
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LifeTime = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Speed = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Direction = FVector::ForwardVector;

protected:
    ARProjectile();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
};
