// Copyright 2020-2022 Aumoa.lib. All right reserved.

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
	APlayerCameraManager* _PlayerCameraManager = nullptr;
	SLocalPlayer* _LocalPlayer = nullptr;

protected:
	SubclassOf<APlayerCameraManager> PlayerCameraManagerClass;

public:
	/// <summary>
	/// Initialize new <see cref="APlayerController"/> instance.
	/// </summary>
	APlayerController();

	SCameraComponent* FindPlayerCameraComponent();
	void UpdateCameraManager(float elapsedTime);
	Ray<3> ScreenPointToRay(int32 screenX, int32 screenY);
	SLocalPlayer* GetLocalPlayer();
	APlayerCameraManager* GetPlayerCameraManager();

public:
	virtual void PostInitializedComponents() override;

private:
	void SpawnPlayerCameraManager();
};