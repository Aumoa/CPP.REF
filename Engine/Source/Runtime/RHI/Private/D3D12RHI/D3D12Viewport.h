// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIViewport.h"

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12Common.h"

class ND3D12Texture2D;

class ND3D12Viewport : public NRHIViewport
{
	static constexpr size_t NumBuffers = 3;

	ComPtr<ID3D12Device> pDevice;
	ComPtr<IDXGISwapChain4> SwapChain4;
	std::vector<std::shared_ptr<ND3D12Texture2D>> Buffers;
	ComPtr<ID3D12DescriptorHeap> RTVHeap;
	uint32 RTVIncrementSize = 0;
	Vector2N VpSize;

public:
	ND3D12Viewport(IDXGIFactory7* InFactory, ID3D12CommandQueue* InQueue, HWND hWnd);
	virtual ~ND3D12Viewport() noexcept override;

	virtual void Present() override;
	virtual Vector2N GetViewportSize() const override;

	int32 GetCurrentBackBufferIndex() const { return SwapChain4->GetCurrentBackBufferIndex(); }
	const ND3D12Texture2D& GetBackBuffer(int32 InIndex) const { return *Buffers[InIndex]; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVHandle(int32 InIndex) const;

private:
	void AllocateBuffersAndCreateDescriptors();
};

#endif