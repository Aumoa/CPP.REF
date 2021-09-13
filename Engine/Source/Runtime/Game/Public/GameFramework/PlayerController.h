// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Controller.h"

class APlayerCameraManager;
class InputComponent;
class SCameraComponent;
class SLocalPlayer;

/// <summary>
/// Represents controller that possess to pawn, process player input, etc...
/// </summary>
class GAME_API APlayerController : public AController
{
	friend class SGameInstance;

public:
	using Super = AController;

private:
	APlayerCameraManager* _cameraManager = nullptr;
	SLocalPlayer* _localPlayer = nullptr;

public:
	/// <summary>
	/// Initialize new <see cref="APlayerController"/> instance.
	/// </summary>
	APlayerController();

	SCameraComponent* FindPlayerCameraComponent() const;
	void SpawnCameraManager(World* level);
	void UpdateCameraManager(std::chrono::duration<float> elapsedTime);
	Ray<3> ScreenPointToRay(int32 screenX, int32 screenY) const;
	void SetLocalPlayer(SLocalPlayer* localPlayer);

	inline SLocalPlayer* GetLocalPlayer() const { return _localPlayer; }
};