// Copyright 2020 Aumoa.lib. All right reserved.

#include "Framework/SpectatorPawn.h"

#include "Components/InputComponent.h"
#include "Components/FloatingPawnMovementComponent.h"
#include "Components/CameraComponent.h"
#include "Framework/PlayerController.h"

ASpectatorPawn::ASpectatorPawn() : Super()
	, bMoveForward(false)
	, bMoveBackward(false)
	, bMoveRight(false)
	, bMoveLeft(false)

	, yaw(0)
	, pitch(0)

	, movementComponent(nullptr)

	, RotationSpeed(10.0f)
{
	PrimaryActorTick.bCanEverTick = true;

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

	inPlayerInput->GetKeyActionBinder(EKey::W, EKeyEvent::Pressed).AddRaw(this, &ASpectatorPawn::HandlePlayerKeyboardInput);
	inPlayerInput->GetKeyActionBinder(EKey::W, EKeyEvent::Released).AddRaw(this, &ASpectatorPawn::HandlePlayerKeyboardInput);
	inPlayerInput->GetKeyActionBinder(EKey::A, EKeyEvent::Pressed).AddRaw(this, &ASpectatorPawn::HandlePlayerKeyboardInput);
	inPlayerInput->GetKeyActionBinder(EKey::A, EKeyEvent::Released).AddRaw(this, &ASpectatorPawn::HandlePlayerKeyboardInput);
	inPlayerInput->GetKeyActionBinder(EKey::S, EKeyEvent::Pressed).AddRaw(this, &ASpectatorPawn::HandlePlayerKeyboardInput);
	inPlayerInput->GetKeyActionBinder(EKey::S, EKeyEvent::Released).AddRaw(this, &ASpectatorPawn::HandlePlayerKeyboardInput);
	inPlayerInput->GetKeyActionBinder(EKey::D, EKeyEvent::Pressed).AddRaw(this, &ASpectatorPawn::HandlePlayerKeyboardInput);
	inPlayerInput->GetKeyActionBinder(EKey::D, EKeyEvent::Released).AddRaw(this, &ASpectatorPawn::HandlePlayerKeyboardInput);
	inPlayerInput->GetCursorMoveBinder().AddRaw(this, &ASpectatorPawn::HandlePlayerCursorInput);
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

void ASpectatorPawn::AddYawInput(TDegrees<float> input)
{
	yaw += input * RotationSpeed;
}

void ASpectatorPawn::AddPitchInput(TDegrees<float> input)
{
	pitch += input * RotationSpeed;
}

void ASpectatorPawn::HandlePlayerKeyboardInput(EKey inKey, EKeyEvent inEvent)
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

void ASpectatorPawn::HandlePlayerCursorInput(const CursorState& inCursorState, const CursorCompare& inCursorDelta)
{
	APlayerController* const playerController = GetController<APlayerController>();

	if (playerController != nullptr && playerController->IsCursorLocked)
	{
		AddYawInput(inCursorDelta.GetDeltaXByDpi());
		AddPitchInput(inCursorDelta.GetDeltaYByDpi());
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

	RootComponent->Rotation = ConsumeRotationInput();
}

Quaternion ASpectatorPawn::ConsumeRotationInput()
{
	Quaternion r = Quaternion::Identity;
	Vector3 right = Vector3::Right;

	yaw = yaw.Normalized;
	pitch = Math::Clamp(pitch.Value, -89.9f, 89.9f);
	
	if (Math::Abs(yaw.Value) > Math::SmallNumber<>)
	{
		r = Quaternion::FromAxisAngle(Vector3::Up, yaw);
		right = r.RotateVector(right);
	}

	if (Math::Abs(pitch.Value) > Math::SmallNumber<>)
	{
		r = Quaternion::Concatenate(r, Quaternion::FromAxisAngle(right, pitch.Value));
	}

	return r;
}