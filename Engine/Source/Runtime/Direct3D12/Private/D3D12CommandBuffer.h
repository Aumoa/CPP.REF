// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommandBuffer.h"
#include "D3D12Common.h"
#include "Graphics.h"
#include "D3D12CommandBuffer.gen.h"

namespace Ayla
{
	class D3D12Graphics;
	class D3D12RaytracingRenderPipeline;

	ACLASS()
	class D3D12CommandBuffer : public CommandBuffer
	{
		GENERATED_BODY()

	private:
		D3D12Graphics* m_Graphics;
		ComPtr<ID3D12CommandAllocator> m_CommandPool;
		std::array<ComPtr<ID3D12GraphicsCommandList4>, Graphics::kMaxFramesInFlight> m_CommandBuffers;
		D3D12RaytracingRenderPipeline* m_CurrentRaytracingRenderPipeline = nullptr;
		uint64 m_FenceValue;

	public:
		D3D12CommandBuffer(D3D12Graphics* graphics);
		virtual ~D3D12CommandBuffer() noexcept override;

		virtual void Dispose() noexcept override;

		virtual void BeginCommands_Implementation() override;
		virtual void EndCommands_Implementation() override;
		virtual void BeginRenderPass(RenderTexture* renderTexture) override;
		virtual void EndRenderPass(RenderTexture* renderTexture) override;
		virtual void SetRenderPipeline(RenderPipeline* renderPipeline) override;
		virtual void Draw(Buffer* vertexBuffer, Buffer* indexBuffer) override;
		virtual void DispatchRays(RenderTexture* renderTexture) override;
		virtual void WaitForCompletion(const TimeSpan& timeout) override;

		ID3D12GraphicsCommandList* GetCommandBuffer() const noexcept;
	};
}
