// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "Pawn.h"

class GFloatingPawnMovementComponent;
class KeyStateTracker;
class MouseStateTracker;
enum class EKey;
enum class EKeyEvent;
struct MouseState;

class GAME_API ASpectatorPawn : public APawn
{
public:
	using Super = APawn;
	using This = ASpectatorPawn;

private:
	bool bMoveForward : 1;
	bool bMoveBackward : 1;
	bool bMoveRight : 1;
	bool bMoveLeft : 1;

	TDegrees<float> yaw;
	TDegrees<float> pitch;

	GFloatingPawnMovementComponent* movementComponent;
	KeyStateTracker* keyTracker;
	MouseStateTracker* mouseTracker;

public:
	ASpectatorPawn();
	~ASpectatorPawn() override;

	void Tick(Seconds deltaTime) override;

	void SetupPlayerInputComponent(GInputComponent* inPlayerInput) override;

	void MoveForward(float value);
	void MoveRight(float value);
	void AddYawInput(TDegrees<float> input);
	void AddPitchInput(TDegrees<float> input);

	float RotationSpeed;

private:
	void HandlePlayerKeyboardInput();
	void HandlePlayerCursorInput();
	void ProcessPlayerInput();

	Quaternion ConsumeRotationInput();
};