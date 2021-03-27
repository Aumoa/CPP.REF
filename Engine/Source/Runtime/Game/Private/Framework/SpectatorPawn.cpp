// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Framework/SpectatorPawn.h"

#include "Components/InputComponent.h"
#include "Components/FloatingPawnMovementComponent.h"
#include "Components/CameraComponent.h"
#include "Framework/PlayerController.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

ASpectatorPawn::ASpectatorPawn() : Super()
	, bMoveForward(false)
	, bMoveBackward(false)
	, bMoveRight(false)
	, bMoveLeft(false)

	, yaw(0)
	, pitch(0)

	, movementComponent(nullptr)
	, keyTracker(nullptr)
	, mouseTracker(nullptr)

	, RotationSpeed(10.0f)
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = AddComponent<GCameraComponent>();
	movementComponent = AddComponent<GFloatingPawnMovementComponent>();
	movementComponent->UpdatedComponent = RootComponent;
}

ASpectatorPawn::~ASpectatorPawn()
{

}

void ASpectatorPawn::Tick(Seconds deltaTime)
{
	Super::Tick(deltaTime);

	if (keyTracker != nullptr || mouseTracker != nullptr)
	{
		HandlePlayerKeyboardInput();
		HandlePlayerCursorInput();
		ProcessPlayerInput();
	}
}

void ASpectatorPawn::SetupPlayerInputComponent(GInputComponent* inPlayerInput)
{
	Super::SetupPlayerInputComponent(inPlayerInput);

	keyTracker = inPlayerInput->KeyTracker;
	mouseTracker = inPlayerInput->MouseTracker;
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

void ASpectatorPawn::HandlePlayerKeyboardInput()
{
	auto last = keyTracker->Last;
	bMoveForward = last.IsKeyDown(EKey::W);
	bMoveLeft = last.IsKeyDown(EKey::A);
	bMoveBackward = last.IsKeyDown(EKey::S);
	bMoveRight = last.IsKeyDown(EKey::D);
}

void ASpectatorPawn::HandlePlayerCursorInput()
{
	MouseState last = mouseTracker->Last;

	if (last.Mode == EMousePositionMode::Relative)
	{
		float yawInput = last.X / 96.0f;
		float pitchInput = last.Y / 96.0f;

		AddYawInput(yawInput * 2.0f);
		AddPitchInput(pitchInput * 2.0f);
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