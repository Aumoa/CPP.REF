// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "IEngineTick.h"

#include <chrono>
#include "Logging/LogCategoryBase.h"

interface IRHIBundle;
interface IRHICommandFence;
interface IRHIDeviceBundle;
interface IRHIImmediateCommandList;
interface IRHISwapChain;

class SceneRenderer;
class GameInstance;
class AssetManager;

class GAME_API Engine : virtual public Object, virtual public IEngineTick
{
public:
	using Super = Object;
	using This = Engine;

private:
	static Engine* gEngine;

	LogCategoryBase LogEngine;
	GameInstance* gameInstance;
	std::chrono::steady_clock::time_point prev_tick;

	std::vector<TRefPtr<IRHIBundle>> rhiBundles;
	TRefPtr<IRHICommandFence> autoFence;

	IRHIDeviceBundle* deviceBundle;
	IRHIImmediateCommandList* immediateCommandList;
	IRHISwapChain* swapChain;

	TRefPtr<SceneRenderer> sceneRenderer;
	TRefPtr<AssetManager> assetManager;
	bool bPresent : 1;

public:
	Engine();
	~Engine() override;

	virtual void Initialize();
	virtual void PostInitialize();
	virtual void Tick();

	vs_property_get(IRHIDeviceBundle*, DeviceBundle);
	IRHIDeviceBundle* DeviceBundle_get() const;
	AssetManager* GetAssetManager() const;

	static Engine* GetInstance();

private:
	void ForEachBundles(std::function<void(IRHIBundle*)> action);
	void LoadEngineDefaultAssets();
	void InitializeDefaultShaders();
};