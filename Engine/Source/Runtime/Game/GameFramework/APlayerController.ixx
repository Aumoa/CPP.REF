// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:APlayerController;

import std.core;
import SC.Runtime.Core;
import :AController;
import :MinimalViewInfo;

using namespace std;
using namespace std::chrono;

export class CameraComponent;
export class APlayerCameraManager;
export class World;

/// <summary>
/// Represents controller that possess to pawn, process player input, etc...
/// </summary>
export class APlayerController : public AController
{
public:
	using Super = AController;

private:
	APlayerCameraManager* _cameraManager = nullptr;

public:
	/// <summary>
	/// Initialize new <see cref="APlayerController"/> instance.
	/// </summary>
	APlayerController();

	CameraComponent* FindPlayerCameraComponent() const;
	void SpawnCameraManager(World* level);
	void UpdateCameraManager(duration<float> elapsedTime);
};