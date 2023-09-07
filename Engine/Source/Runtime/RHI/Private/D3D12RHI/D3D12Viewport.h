// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIViewport.h"

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12Common.h"

class ND3D12Viewport : public NRHIViewport
{
	ComPtr<IDXGISwapChain4> SwapChain4;

public:
	ND3D12Viewport(IDXGIFactory7* InFactory, ID3D12CommandQueue* InQueue, HWND hWnd);
	virtual ~ND3D12Viewport() noexcept override;

	virtual void Present() override;
};

#endif