// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "RHI/RHIGraphics.h"

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12Common.h"

class ND3D12Graphics : public NRHIGraphics
{
	ComPtr<IDXGIFactory7> DXGIFactory;
	ComPtr<IDXGIAdapter1> CurrentAdapter;
	ComPtr<ID3D12Device1> Device;

	// FrameResources
	ComPtr<ID3D12Fence> Fence;
	HANDLE hFenceEvent = nullptr;
	uint64 FenceValue = 0;

	// AsyncCommands
	Spinlock AsyncLock;
	std::vector<Action<ID3D12GraphicsCommandList*>> AsyncCommands;
	size_t AsyncIndex = 0;
	TaskCompletionSource<> AsyncPulse[2];
	ComPtr<ID3D12CommandAllocator> AsyncCommandAllocator;
	ComPtr<ID3D12GraphicsCommandList> AsyncCommandList;

public:
	ND3D12Graphics();
	virtual ~ND3D12Graphics() noexcept override;

	virtual void Init() override;
	virtual std::shared_ptr<NRHICommandQueue> CreateCommandQueue() override;
	virtual std::shared_ptr<NRHIViewport> CreateViewport(NRHICommandQueue& InCommandQueue, NGenericWindow& InWindow) override;
	virtual Task<std::shared_ptr<NRHITexture2D>> CreateTexture2DAsync(std::shared_ptr<NGenericImage> ImageSource) override;

	virtual void BeginFrame() override;
	virtual void EndFrame() override;

	ID3D12Device1* GetDevice() const;

	Task<> EnqueueGraphicsCommandAsync(Action<ID3D12GraphicsCommandList*> InAction);

public:
	static std::unique_ptr<NRHIGraphics> GenerateGraphics();

private:
	void PulseAsyncCommands();
};

#endif