#pragma once

#include <GameFramework/Character.h>
#include "RShip.generated.h"

UCLASS()
class REIKONOIDS_API ARShip : public ACharacter
{
	GENERATED_BODY()

public:
	void Fire();
	void MoveForward(float AxisScale);
	void RotateRight(float AxisScale);

protected:
	ARShip();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


};
