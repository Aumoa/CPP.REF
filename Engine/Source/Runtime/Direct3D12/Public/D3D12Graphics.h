// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Graphics.h"
#include "D3D12Common.h"
#include "D3D12Graphics.gen.h"

namespace Ayla
{
	class CommandQueue;

	ACLASS()
	class DIRECT3D12_API D3D12Graphics : public Graphics
	{
		GENERATED_BODY()

	private:
		ComPtr<IDXGIFactory5> m_DXGI;
		ComPtr<ID3D12Device5> m_Device;
		std::array<std::unique_ptr<CommandQueue>, 3> m_CommandQueue;
		std::array<uint64, 2> m_LastFrameFenceValue;

	public:
		ACONSTRUCTOR()
		D3D12Graphics();
		virtual ~D3D12Graphics() noexcept override;

		virtual void Dispose() noexcept override;
		virtual RenderFeatures GetCurrentRenderFeature() noexcept override { return RenderFeatures::D3D12; }

		virtual SharedPtr<GenericWindowSwapchainExtension> InstallSwapChain(SharedPtr<GenericWindow> targetWindow) override;
		virtual SharedPtr<CommandBuffer> CreateCommandBuffer() override;
		virtual SharedPtr<GraphicsRenderPipeline> CreateGeometryRenderPipeline(SharedPtr<Shader> shader) override;
		virtual SharedPtr<RaytracingRenderPipeline> CreateRaytracingRenderPipeline(SharedPtr<Shader> shader) override;
		virtual SharedPtr<Shader> CreateShader(ShaderCreationInfo shaderCreationInfo) override;
		virtual SharedPtr<Buffer> CreateUploadBuffer(size_t sizeInBytes) override;
		virtual SharedPtr<Buffer> CreateBuffer(std::span<const byte> data, size_t stride, BufferUsage usage) override;

		virtual void BeginRenderFrame() override;
		virtual void EndRenderFrame() override;
		virtual void WaitForCompletion() override;

		inline ID3D12Device1* GetDevice() const noexcept { return m_Device.Get(); }
		inline CommandQueue& GetCommandQueue() const noexcept { return *m_CommandQueue[0]; }
	};
}
