// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "RHI/RHIInterfaces.h"
#include "DirectXCommon.h"

class DIRECTX_API SDirectXFactory : implements SObject, implements IRHIFactory
{
	GENERATED_BODY(SDirectXFactory)

public:
	ComPtr<IDXGIFactory4> pFactory;

public:
	SDirectXFactory(bool bEnableDebugLayer = false);

	virtual void Dispose() override;

	virtual IRHIAdapter* GetAdapter(size_t index) override;
	virtual IRHIDevice* CreateDevice(IRHIAdapter* pAdapter) override;
	virtual IRHISwapChain* CreateSwapChain(IRHICommandQueue* pQueue, size_t numBuffers) override;

protected:
	virtual void Dispose(bool bDisposing) override;
};