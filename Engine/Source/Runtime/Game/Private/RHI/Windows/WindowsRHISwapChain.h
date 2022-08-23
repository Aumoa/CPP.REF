// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHISwapChain.h"

#if PLATFORM_WINDOWS

#include "WindowsRHICommon.h"

class WindowsRHIDevice;
class WindowsRHICommandQueue;
class WindowsRHIResource;
interface IPlatformWindow;

class WindowsRHISwapChain : public RHISwapChain
{
	friend WindowsRHIDevice;

private:
	ComPtr<IDXGISwapChain3> _swapChain;
	std::vector<std::shared_ptr<WindowsRHIResource>> _buffers;

private:
	WindowsRHISwapChain(std::shared_ptr<WindowsRHICommandQueue> queue, IPlatformWindow* drawingWindow);

public:
	virtual ~WindowsRHISwapChain() noexcept override;

	virtual void Present() override;
	virtual void ResizeBuffers(const Vector2N& size) override;
	virtual size_t GetCurrentBackBufferIndex() const override;
	virtual std::shared_ptr<RHIResource> GetBuffer(size_t index) override;

private:
	void AssignBuffers();
};

#endif