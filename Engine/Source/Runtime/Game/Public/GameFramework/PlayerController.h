// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Controller.h"

class APlayerCameraManager;
class InputComponent;
class CameraComponent;
class LocalPlayer;

/// <summary>
/// Represents controller that possess to pawn, process player input, etc...
/// </summary>
class GAME_API APlayerController : public AController
{
	friend class GameInstance;

public:
	using Super = AController;

private:
	APlayerCameraManager* _cameraManager = nullptr;
	InputComponent* _inputComponent = nullptr;
	LocalPlayer* _localPlayer = nullptr;

public:
	/// <summary>
	/// Initialize new <see cref="APlayerController"/> instance.
	/// </summary>
	APlayerController();

	CameraComponent* FindPlayerCameraComponent() const;
	void SpawnCameraManager(World* level);
	void UpdateCameraManager(std::chrono::duration<float> elapsedTime);
	Ray<3> ScreenPointToRay(int32 screenX, int32 screenY) const;

	inline InputComponent* GetInputComponent() const { return _inputComponent; }
	inline LocalPlayer* GetLocalPlayer() const { return _localPlayer; }
};