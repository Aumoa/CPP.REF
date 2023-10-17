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
	ComPtr<IDWriteFactory> WriteFactory;
	std::shared_ptr<NRHICommandQueue> PrimaryQueue;

	ComPtr<ID3D11Device> Device11;
	ComPtr<ID3D11On12Device> Device11On12;
	ComPtr<ID3D11DeviceContext> DeviceContext;
	ComPtr<ID2D1Device> D2DDevice;

	// FrameResources
	ComPtr<ID3D12Fence> Fence;
	HANDLE hFenceEvent = nullptr;
	uint64 FenceValue = 0;

	// AsyncCommands
	Spinlock AsyncLock;
	std::vector<Func<ID3D12GraphicsCommandList*, bool>> AsyncCommands;
	size_t AsyncIndex = 0;
	TaskCompletionSource<> AsyncPulse[2];
	ComPtr<ID3D12CommandAllocator> AsyncCommandAllocator;
	std::vector<ComPtr<ID3D12GraphicsCommandList>> AsyncCommandLists;

public:
	ND3D12Graphics();
	virtual ~ND3D12Graphics() noexcept override;

	virtual void Init() override;
	virtual std::shared_ptr<NRHICommandQueue> GetPrimaryQueue() override;
	virtual std::shared_ptr<NRHICommandQueue> CreateCommandQueue() override;
	virtual std::shared_ptr<NRHISwapChain> CreateSwapChain(NRHICommandQueue& InCommandQueue, NGenericWindow& InWindow) override;
	virtual std::shared_ptr<NRHIViewport> CreateViewport() override;
	virtual Task<std::shared_ptr<NRHITexture2D>> CreateTexture2DAsync(std::shared_ptr<NGenericImage> ImageSource) override;
	virtual std::shared_ptr<NRHICommandSet> CreateCommandSet() override;
	virtual std::shared_ptr<NRHIRootSignature> CreateRootSignature() override;
	virtual std::shared_ptr<NRHIGraphicsPipelineState> CreateGraphicsPipelineState(NRHIRootSignature& InRS) override;
	virtual std::shared_ptr<NRHIConstantBuffer> CreateConstantBuffer() override;
	virtual std::shared_ptr<NRHIDescriptorHeap> CreateDescriptorHeap() override;
	virtual std::shared_ptr<NRHIShaderResourceView> CreateShaderResourceView(size_t InNumViews) override;
	virtual std::shared_ptr<NRHITextFormat> CreateTextFormat(String FontFamilyName, float FontSize, bool bBold, bool bItalic) override;
	virtual std::shared_ptr<NRHITextLayout> CreateTextLayout(std::shared_ptr<NRHITextFormat> InTextFormat, String InText) override;

	virtual void BeginFrame() override;
	virtual void EndFrame() override;
	virtual void SyncFrame() override;

	inline ID3D12Device1* GetDevice() const { return Device.Get(); }
	inline IDWriteFactory* GetWriteFactory() const { return WriteFactory.Get(); }

	inline ID3D11Device& GetD3D11Device() const { return *Device11.Get(); }
	inline ID3D11On12Device& GetD3D11On12Device() const { return *Device11On12.Get(); }
	inline ID3D11DeviceContext& GetD3D11DeviceContext() const { return *DeviceContext.Get(); }

	ComPtr<ID2D1DeviceContext> CreateDeviceContext2D() const;
	Task<> EnqueueGraphicsCommandAsync(Func<ID3D12GraphicsCommandList*, bool> InAction);

public:
	static std::unique_ptr<NRHIGraphics> GenerateGraphics();

private:
	void PulseAsyncCommands();
};

#endif