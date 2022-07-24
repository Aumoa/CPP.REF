// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHISwapChain.h"

#if PLATFORM_WINDOWS

#include "WindowsRHICommon.h"

class WindowsRHIDevice;
class WindowsRHICommandQueue;
interface IPlatformWindow;

class GAME_API WindowsRHISwapChain : public RHISwapChain
{
	friend WindowsRHIDevice;

private:
	ComPtr<IDXGISwapChain3> _swapChain;

private:
	WindowsRHISwapChain(std::shared_ptr<WindowsRHICommandQueue> queue, IPlatformWindow* drawingWindow);

public:
	virtual ~WindowsRHISwapChain() noexcept override;

	virtual void Present() override;
};

#endif