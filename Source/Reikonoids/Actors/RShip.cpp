#include "RShip.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>

ARShip::ARShip()
{
	// Setup capsule component.
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	check(Capsule != nullptr);

	Capsule->SetCapsuleRadius(50.0f);
	Capsule->SetCapsuleHalfHeight(50.0f);

	// Setup movement component.
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	check(Movement != nullptr);

	Movement->DefaultLandMovementMode = EMovementMode::MOVE_Flying;
	Movement->SetPlaneConstraintNormal(FVector::UpVector);
	Movement->bConstrainToPlane = true;
}

void ARShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &ARShip::Fire);
	PlayerInputComponent->BindAxis("MoveForward", this, &ARShip::MoveForward);
	PlayerInputComponent->BindAxis("RotateRight", this, &ARShip::RotateRight);
}

void ARShip::BeginPlay()
{
	Super::BeginPlay();
}

void ARShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARShip::Fire()
{

}

void ARShip::MoveForward(float AxisScale)
{
	GetCharacterMovement()->AddInputVector(this->GetActorForwardVector() * AxisScale);
}

void ARShip::RotateRight(float AxisScale)
{
	this->AddControllerYawInput(AxisScale);
}
