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
	GENERATED_BODY(APlayerController)

private:
	APlayerCameraManager* _cameraManager = nullptr;
	SLocalPlayer* _localPlayer = nullptr;

public:
	/// <summary>
	/// Initialize new <see cref="APlayerController"/> instance.
	/// </summary>
	APlayerController();

	SCameraComponent* FindPlayerCameraComponent();
	void SpawnCameraManager(SWorld* level);
	void UpdateCameraManager(float elapsedTime);
	Ray<3> ScreenPointToRay(int32 screenX, int32 screenY);
	void SetLocalPlayer(SLocalPlayer* localPlayer);

	inline SLocalPlayer* GetLocalPlayer() const { return _localPlayer; }
};