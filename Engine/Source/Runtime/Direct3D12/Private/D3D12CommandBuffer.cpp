// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "D3D12CommandBuffer.h"
#include "D3D12Graphics.h"
#include "CommandQueue.h"
#include "D3D12RaytracingRenderPipeline.h"
#include "D3D12GeometryRenderPipeline.h"
#include "D3D12Buffer.h"
#include "DXGISwapchainRenderTexture.h"

namespace Ayla
{
	D3D12CommandBuffer::D3D12CommandBuffer(D3D12Graphics* graphics)
		: m_Graphics(graphics)
	{
		HR(graphics->GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandPool)));
		DXSetName(m_CommandPool);

		for (size_t i = 0; i < Graphics::kMaxFramesInFlight; ++i)
		{
			HR(graphics->GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandPool.Get(), nullptr, IID_PPV_ARGS(&m_CommandBuffers[i])));
			DXSetName(m_CommandBuffers[i]);
			HR(m_CommandBuffers[i]->Close());
		}

	}

	D3D12CommandBuffer::~D3D12CommandBuffer() noexcept
	{
	}

	void D3D12CommandBuffer::Dispose() noexcept
	{
		m_CommandPool.Reset();
		for (auto& commandBuffer : m_CommandBuffers)
		{
			commandBuffer.Reset();
		}
	}

	void D3D12CommandBuffer::BeginCommands_Implementation()
	{
		auto pi = m_Graphics->GetFramePageIndex();
		HR(m_CommandPool->Reset());
		HR(m_CommandBuffers[pi]->Reset(m_CommandPool.Get(), nullptr));
	}

	void D3D12CommandBuffer::EndCommands_Implementation()
	{
		auto pi = m_Graphics->GetFramePageIndex();
		HR(m_CommandBuffers[pi]->Close());
		ID3D12CommandList* targetCommandBuffer = m_CommandBuffers[pi].Get();
		auto& queue = m_Graphics->GetCommandQueue();
		queue.GetQueue()->ExecuteCommandLists(1, &targetCommandBuffer);
		m_FenceValue = queue.Signal();
	}

	void D3D12CommandBuffer::BeginRenderPass(RenderTexture* renderTexture)
	{
		auto pi = m_Graphics->GetFramePageIndex();
		if (auto* rt = dynamic_cast<DXGISwapchainRenderTexture*>(renderTexture))
		{
			D3D12_RESOURCE_BARRIER barrier =
			{
				.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
				.Transition =
				{
					.pResource = rt->GetCurrentBackBuffer(),
					.Subresource = 0,
					.StateBefore = D3D12_RESOURCE_STATE_PRESENT,
					.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET
				}
			};
			m_CommandBuffers[pi]->ResourceBarrier(1, &barrier);

			D3D12_RENDER_PASS_RENDER_TARGET_DESC renderTargetDesc =
			{
				.cpuDescriptor = rt->GetRTVDescriptorHandle(),
				.BeginningAccess =
				{
					.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR,
					.Clear = { .ClearValue = { .Format = DXGI_FORMAT_R8G8B8A8_UNORM, .Color = { 0, 0, 0, 1 } } }
				},
				.EndingAccess =
				{
					.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE
				}
			};

			D3D12_RENDER_PASS_DEPTH_STENCIL_DESC depthStencilDesc =
			{
				.cpuDescriptor = rt->GetDSVDescriptorHandle(),
				.DepthBeginningAccess =
				{
					.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR,
					.Clear = { .ClearValue = { .Format = DXGI_FORMAT_D32_FLOAT, .DepthStencil = { 1, 0 } } }
				},
				.StencilBeginningAccess =
				{
					.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR,
					.Clear = { .ClearValue = { .Format = DXGI_FORMAT_D32_FLOAT, .DepthStencil = { 1, 0 } } }
				},
				.DepthEndingAccess =
				{
					.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE
				},
				.StencilEndingAccess =
				{
					.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE
				}
			};

			m_CommandBuffers[pi]->BeginRenderPass(1, &renderTargetDesc, &depthStencilDesc, D3D12_RENDER_PASS_FLAG_NONE);

			auto size = rt->GetSize();
			D3D12_VIEWPORT viewport =
			{
				.TopLeftX = 0,
				.TopLeftY = 0,
				.Width = static_cast<float>(size.X),
				.Height = static_cast<float>(size.Y),
				.MinDepth = 0,
				.MaxDepth = 1
			};
			m_CommandBuffers[pi]->RSSetViewports(1, &viewport);

			D3D12_RECT scissorRect =
			{
				.left = 0,
				.top = 0,
				.right = static_cast<LONG>(size.X),
				.bottom = static_cast<LONG>(size.Y)
			};
			m_CommandBuffers[pi]->RSSetScissorRects(1, &scissorRect);
			return;
		}
		else
		{
			throw InvalidOperationException(TEXT("Unsupported render texture type."));
		}
	}

	void D3D12CommandBuffer::EndRenderPass(RenderTexture* renderTexture)
	{
		if (auto* rt = dynamic_cast<DXGISwapchainRenderTexture*>(renderTexture))
		{
			D3D12_RESOURCE_BARRIER barrier =
			{
				.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
				.Transition =
				{
					.pResource = rt->GetCurrentBackBuffer(),
					.Subresource = 0,
					.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET,
					.StateAfter = D3D12_RESOURCE_STATE_PRESENT
				}
			};

			auto pi = m_Graphics->GetFramePageIndex();
			m_CommandBuffers[pi]->EndRenderPass();
			m_CommandBuffers[pi]->ResourceBarrier(1, &barrier);
		}
	}

	void D3D12CommandBuffer::SetRenderPipeline(RenderPipeline* renderPipeline)
	{
		if (auto* pso1 = dynamic_cast<D3D12RaytracingRenderPipeline*>(renderPipeline))
		{
			auto pi = m_Graphics->GetFramePageIndex();
			m_CommandBuffers[pi]->SetPipelineState1(pso1->GetPipelineStateObject());
			return;
		}
		else if (auto* ps = dynamic_cast<D3D12GeometryRenderPipeline*>(renderPipeline))
		{
			auto pi = m_Graphics->GetFramePageIndex();
			m_CommandBuffers[pi]->SetGraphicsRootSignature(ps->GetRootSignature());
			m_CommandBuffers[pi]->SetPipelineState(ps->GetPipelineState());
			return;
		}
		else
		{
			throw InvalidOperationException(TEXT("Unsupported render pipeline type."));
		}
	}

	void D3D12CommandBuffer::Draw(Buffer* vertexBuffer, Buffer* indexBuffer)
	{
		auto* d3d12VertexBuffer = dynamic_cast<D3D12Buffer*>(vertexBuffer);
		if (d3d12VertexBuffer == nullptr || d3d12VertexBuffer->GetUsage() != BufferUsage::VertexBuffer)
		{
			throw InvalidOperationException(TEXT("Direct3D12 draw requires a Direct3D12 vertex buffer."));
		}

		auto* d3d12IndexBuffer = dynamic_cast<D3D12Buffer*>(indexBuffer);
		if (d3d12IndexBuffer == nullptr || d3d12IndexBuffer->GetUsage() != BufferUsage::IndexBuffer)
		{
			throw InvalidOperationException(TEXT("Direct3D12 draw requires a Direct3D12 index buffer."));
		}

		const size_t indexStride = d3d12IndexBuffer->GetStride();
		if (indexStride != sizeof(uint16) && indexStride != sizeof(uint32))
		{
			throw InvalidOperationException(TEXT("Direct3D12 draw requires a 16-bit or 32-bit index buffer."));
		}

		auto vertexBufferView = d3d12VertexBuffer->GetVertexBufferView();
		auto indexBufferView = d3d12IndexBuffer->GetIndexBufferView();
		
		auto pi = m_Graphics->GetFramePageIndex();
		m_CommandBuffers[pi]->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandBuffers[pi]->IASetVertexBuffers(0, 1, &vertexBufferView);
		m_CommandBuffers[pi]->IASetIndexBuffer(&indexBufferView);
		m_CommandBuffers[pi]->DrawIndexedInstanced(static_cast<UINT>(d3d12IndexBuffer->GetCount()), 1, 0, 0, 0);
	}

	void D3D12CommandBuffer::DispatchRays(RenderTexture* renderTexture)
	{
		throw InvalidOperationException(TEXT("Direct3D12 ray dispatch is not implemented yet."));
	}

	void D3D12CommandBuffer::WaitForCompletion(const TimeSpan& timeout)
	{
		auto& queue = m_Graphics->GetCommandQueue();
		queue.WaitForCompletion(m_FenceValue, timeout);
	}

	ID3D12GraphicsCommandList* D3D12CommandBuffer::GetCommandBuffer() const noexcept
	{
		auto pi = m_Graphics->GetFramePageIndex();
		return m_CommandBuffers[pi].Get();
	}
}
