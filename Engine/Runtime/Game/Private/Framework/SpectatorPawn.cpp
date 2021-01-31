// Copyright 2020 Aumoa.lib. All right reserved.

#include "Framework/SpectatorPawn.h"

#include "Components/InputComponent.h"
#include "Components/FloatingPawnMovementComponent.h"
#include "Components/CameraComponent.h"

ASpectatorPawn::ASpectatorPawn() : Super()
	, bMoveForward(false)
	, bMoveBackward(false)
	, bMoveRight(false)
	, bMoveLeft(false)

	, movementComponent(nullptr)
{
	RootComponent = AddComponent<CameraComponent>();
	movementComponent = AddComponent<FloatingPawnMovementComponent>();
	movementComponent->UpdatedComponent = RootComponent;
}

ASpectatorPawn::~ASpectatorPawn()
{

}

void ASpectatorPawn::Tick(Seconds deltaTime)
{
	Super::Tick(deltaTime);

	ProcessPlayerInput();
}

void ASpectatorPawn::SetupPlayerInputComponent(InputComponent* inPlayerInput)
{
	Super::SetupPlayerInputComponent(inPlayerInput);

	inPlayerInput->GetKeyActionBinder(EKey::W, EKeyEvent::Pressed).AddRaw(this, &ASpectatorPawn::HandlePlayerInput);
	inPlayerInput->GetKeyActionBinder(EKey::W, EKeyEvent::Released).AddRaw(this, &ASpectatorPawn::HandlePlayerInput);
	inPlayerInput->GetKeyActionBinder(EKey::A, EKeyEvent::Pressed).AddRaw(this, &ASpectatorPawn::HandlePlayerInput);
	inPlayerInput->GetKeyActionBinder(EKey::A, EKeyEvent::Released).AddRaw(this, &ASpectatorPawn::HandlePlayerInput);
	inPlayerInput->GetKeyActionBinder(EKey::S, EKeyEvent::Pressed).AddRaw(this, &ASpectatorPawn::HandlePlayerInput);
	inPlayerInput->GetKeyActionBinder(EKey::S, EKeyEvent::Released).AddRaw(this, &ASpectatorPawn::HandlePlayerInput);
	inPlayerInput->GetKeyActionBinder(EKey::D, EKeyEvent::Pressed).AddRaw(this, &ASpectatorPawn::HandlePlayerInput);
	inPlayerInput->GetKeyActionBinder(EKey::D, EKeyEvent::Released).AddRaw(this, &ASpectatorPawn::HandlePlayerInput);
}

void ASpectatorPawn::MoveForward(float value)
{
	Quaternion rotation = RootComponent->ComponentRotation;
	Vector3 myForward = rotation.RotateVector(Vector3::Forward);
	movementComponent->AddInputVector(myForward * value);
}

void ASpectatorPawn::MoveRight(float value)
{
	Quaternion rotation = RootComponent->ComponentRotation;
	Vector3 myRight = rotation.RotateVector(Vector3::Right);
	movementComponent->AddInputVector(myRight * value);
}

void ASpectatorPawn::HandlePlayerInput(EKey inKey, EKeyEvent inEvent)
{
	switch (inKey)
	{
	case EKey::W:
		bMoveForward = inEvent == EKeyEvent::Pressed;
		break;
	case EKey::A:
		bMoveLeft = inEvent == EKeyEvent::Pressed;
		break;
	case EKey::S:
		bMoveBackward = inEvent == EKeyEvent::Pressed;
		break;
	case EKey::D:
		bMoveRight = inEvent == EKeyEvent::Pressed;
		break;
	}
}

void ASpectatorPawn::ProcessPlayerInput()
{
	Vector3 inputVector = Vector3::Zero;

	if (bMoveForward)
	{
		MoveForward(1.0f);
	}
	if (bMoveBackward)
	{
		MoveForward(-1.0f);
	}
	if (bMoveRight)
	{
		MoveRight(1.0f);
	}
	if (bMoveLeft)
	{
		MoveRight(-1.0f);
	}
}