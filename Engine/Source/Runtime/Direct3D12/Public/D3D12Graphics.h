// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Graphics.h"
#include "D3D12Common.h"
#include "D3D12Graphics.gen.h"

namespace Ayla
{
	ACLASS()
	class DIRECT3D12_API D3D12Graphics : public Graphics
	{
		GENERATED_BODY()

	private:
		ComPtr<IDXGIFactory5> m_DXGI;
		ComPtr<ID3D12Device1> m_Device;
		ComPtr<ID3D12CommandQueue> m_CommandQueue;
		ComPtr<ID3D12Fence> m_Fence;
		HANDLE m_FenceEvent = NULL;
		std::atomic<uint64> m_FenceValue = 0;

		std::atomic<std::size_t> m_FrameCount = 0;

	public:
		ACONSTRUCTOR()
		D3D12Graphics();
		virtual ~D3D12Graphics() noexcept override;

		virtual void Dispose() noexcept override;
		virtual RenderFeatures GetCurrentRenderFeature() noexcept override { return RenderFeatures::D3D12; }

		virtual SharedPtr<GenericWindowSwapchainExtension> InstallSwapChain_Implementation(SharedPtr<GenericWindow> targetWindow) override;
		virtual SharedPtr<CommandBuffer> CreateCommandBuffer_Implementation() override;

		virtual void BeginRenderFrame() override;
		virtual void EndRenderFrame() override;
		virtual void WaitForCompletion() override;

		inline size_t GetFrameNumber() const noexcept { return m_FrameCount; }
		inline ID3D12Device1* GetDevice() const noexcept { return m_Device.Get(); }
		inline ID3D12CommandQueue* GetCommandQueue() const noexcept { return m_CommandQueue.Get(); }
	};
}