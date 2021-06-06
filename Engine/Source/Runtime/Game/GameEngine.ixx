// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:GameEngine;

import SC.Runtime.Core;
import SC.Runtime.RenderCore;
import SC.Runtime.Game.Shaders;

export class GameInstance;

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
	ColorShader* _colorShader = nullptr;

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
	void TickEngine();
	void ResizedApp(int32 width, int32 height);
};