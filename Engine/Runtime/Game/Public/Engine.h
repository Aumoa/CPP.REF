// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

class GameInstance;
class DirectXDeviceBundle;
class DirectXCommandQueue;
class DirectXSwapChain;
class DirectXImmediateContext;
class DirectXAutoFence;

class GAME_API Engine : virtual public Object
{
public:
	using Super = Object;
	using This = Engine;

private:
	GameInstance* gameInstance;
	TRefPtr<DirectXDeviceBundle> deviceBundle;
	TRefPtr<DirectXCommandQueue> primaryQueue;

	TRefPtr<DirectXSwapChain> swapChain;
	TRefPtr<DirectXImmediateContext> immediateContext;
	TRefPtr<DirectXAutoFence> immediateFence;

public:
	Engine();
	~Engine() override;

	virtual void Initialize(GameInstance* gameInstance);
	virtual void Shutdown();
	virtual void Tick();

private:
	void Render();

private:
	void MainWindow_OnSizing(int32 x, int32 y);
};