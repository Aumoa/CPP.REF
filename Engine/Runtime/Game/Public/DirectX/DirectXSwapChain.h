// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "DirectXMinimal.h"

class DirectXDeviceBundle;
class DirectXCommandQueue;
class CoreWindow;

class GAME_API DirectXSwapChain : virtual public Object
{
public:
	using Super = Object;
	
private:
	TComPtr<IDXGISwapChain4> swapChain;

public:
	DirectXSwapChain(DirectXDeviceBundle* deviceBundle, DirectXCommandQueue* commandQueue, CoreWindow* target, DXGI_FORMAT format);
	~DirectXSwapChain() override;

	void Present();
	void ResizeBuffers(int32 x, int32 y);
};