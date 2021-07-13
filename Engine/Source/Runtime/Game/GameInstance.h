// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameObject.h"
#include <chrono>
#include "SubclassOf.h"

struct IFrameworkView;
class GameEngine;
class Level;
class LocalPlayer;

/// <summary>
/// Represents single game instance while application are running.
/// </summary>
class GameInstance : public GameObject
{
public:
	using Super = GameObject;
	using PostInitializedEvent = MulticastEvent<GameInstance, void()>;

private:
	IFrameworkView* _frameworkView = nullptr;
	GameEngine* _engine = nullptr;
	World* _world = nullptr;
	LocalPlayer* _localPlayer = nullptr;

public:
	SubclassOf<Level> StartupLevel;

public:
	/// <summary>
	/// Initialize new <see cref="GameInstance"/> instance.
	/// </summary>
	GameInstance();

	virtual void Tick(std::chrono::duration<float> elapsedTime);

	/// <summary>
	/// Run the application.
	/// </summary>
	/// <param name="frameworkView"> The core framework view window. </param>
	/// <returns> Return primary exit code. </returns>
	int32 Run(IFrameworkView* frameworkView);

	/// <summary>
	/// Event for game engine be initialized.
	/// </summary>
	PostInitializedEvent PostInitialized;

	/// <summary>
	/// Get core framework view.
	/// </summary>
	IFrameworkView* GetFrameworkView() const;

protected:
	/// <summary>
	/// Initialize the game engine.
	/// </summary>
	virtual void InitializeEngine();

public:
	inline LocalPlayer* GetLocalPlayer() const { return _localPlayer; }
};