// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:GameEngine;

import SC.Runtime.Core;
import SC.Runtime.RenderCore;
import SC.Runtime.Game.Shaders;
import :TickScheduler;
import std.core;

export class GameInstance;

using namespace std;
using namespace std::chrono;

/// <summary>
/// Represents game engine that manage core resources.
/// </summary>
export class GameEngine : virtual public Object
{
public:
	using Super = Object;

private:
	const uint8 _bDebug : 1;
	
	GameInstance* _gameInstance = nullptr;
	RHIDevice* _device = nullptr;
	RHICommandQueue* _primaryQueue = nullptr;
	RHISwapChain* _frameworkViewChain = nullptr;
	RHIDeviceContext* _deviceContext = nullptr;
	ColorVertexFactory* _colorVertexFactory = nullptr;
	ColorShader* _colorShader = nullptr;
	RHIRenderTargetView* _rtv = nullptr;

	int32 _vpWidth = 0;
	int32 _vpHeight = 0;

	TickScheduler _scheduler;

	optional<steady_clock::time_point> _prev;

public:
	/// <summary>
	/// Initialize new <see cref="GameEngine"/> instance.
	/// <summary>
	/// <param name="bDebug"/> Make application state to debugging. </param>
	GameEngine(bool bDebug);
	~GameEngine() override;

	/// <summary>
	/// Initialize engine system.
	/// </summary>
	/// <param name="gameInstance"> The owner game instance. </param>
	virtual void InitEngine(GameInstance* gameInstance);

private:
	static GameEngine* _gEngine;

public:
	static GameEngine* GetEngine() { return _gEngine; }

private:
	void RegisterRHIGarbageCollector();
	void TickEngine();
	void ResizedApp(int32 width, int32 height);

private:
	void GameTick(duration<float> elapsedTime);
	void RenderTick(duration<float> elapsedTime);

public:
	RHIDevice* GetRHIDevice() const { return _device; }
	ColorVertexFactory* GetColorVertexFactory() const { return _colorVertexFactory; }
};