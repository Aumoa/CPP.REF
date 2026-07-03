// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12Common.h"
#include "Rendering/PresentableRenderTarget.h"
#include "Rendering/RenderTexture.h"
#include "DXGISwapchainRenderTexture.gen.h"

namespace Ayla
{
	ACLASS()
	class DXGISwapchainRenderTexture : public RenderTexture, public PresentableRenderTarget
	{
		GENERATED_BODY()

	private:
		ID3D12Device* m_Device;
		ComPtr<IDXGISwapChain3> m_Swapchain;
		DXGI_SWAP_CHAIN_DESC m_SwapchainDesc;
		std::vector<ComPtr<ID3D12Resource>> m_SwapchainResources;
		ComPtr<ID3D12Resource> m_DepthStencilResource;
		ComPtr<ID3D12Resource> m_RaytracingOutputResource;
		uint32 m_CurrentBackBufferIndex = 0;
		D3D12_RESOURCE_STATES m_RaytracingOutputState = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;

		ComPtr<ID3D12DescriptorHeap> m_RTVHeap;
		ComPtr<ID3D12DescriptorHeap> m_DSVHeap;
		ComPtr<ID3D12DescriptorHeap> m_RaytracingOutputUAVHeap;

	public:
		DXGISwapchainRenderTexture(ID3D12Device* device, ComPtr<IDXGISwapChain3> swapchain);
		virtual ~DXGISwapchainRenderTexture() noexcept override;

		virtual Vector2N GetSize() const override;

		virtual PresentableFrame AcquireFrame(CommandBuffer* commandBuffer) override;
		virtual void Present(CommandBuffer* commandBuffer) override;
		virtual void Invalidate() override;

		void ReleaseResources();
		void AllocateResources(bool resize);

		ID3D12Resource* GetCurrentBackBuffer() const;
		ID3D12Resource* GetRaytracingOutputResource() const;
		ID3D12DescriptorHeap* GetRaytracingOutputDescriptorHeap() const;
		D3D12_CPU_DESCRIPTOR_HANDLE GetRTVDescriptorHandle() const;
		D3D12_CPU_DESCRIPTOR_HANDLE GetDSVDescriptorHandle() const;
		D3D12_GPU_DESCRIPTOR_HANDLE GetRaytracingOutputUAVGPUDescriptorHandle() const;
		D3D12_RESOURCE_STATES GetRaytracingOutputState() const noexcept { return m_RaytracingOutputState; }
		void SetRaytracingOutputState(D3D12_RESOURCE_STATES state) noexcept { m_RaytracingOutputState = state; }
	};
}
