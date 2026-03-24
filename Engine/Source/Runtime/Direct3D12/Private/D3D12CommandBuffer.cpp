// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "D3D12CommandBuffer.h"
#include "D3D12Graphics.h"
#include "CommandQueue.h"
#include "D3D12RaytracingRenderPipeline.h"
#include "D3D12GeometryRenderPipeline.h"
#include "DXGISwapchainRenderTexture.h"
#include "Misc/PositionColorVertex.h"

namespace Ayla
{
	D3D12CommandBuffer::D3D12CommandBuffer(D3D12Graphics* graphics)
		: m_Graphics(graphics)
	{
		HR(graphics->GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandPool)));
		DXSetName(m_CommandPool);
		HR(graphics->GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandPool.Get(), nullptr, IID_PPV_ARGS(&m_CommandBuffer)));
		DXSetName(m_CommandBuffer);
		HR(m_CommandBuffer->Close());

		D3D12_HEAP_PROPERTIES heapProp = { D3D12_HEAP_TYPE_UPLOAD };
		D3D12_RESOURCE_DESC resourceDesc =
		{
			.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER,
			.Alignment = 0,
			.Width = sizeof(PositionColorVertex) * 3,
			.Height = 1,
			.DepthOrArraySize = 1,
			.MipLevels = 1,
			.Format = DXGI_FORMAT_UNKNOWN,
			.SampleDesc =
			{
				 .Count = 1,
				 .Quality = 0
			},
			.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
			.Flags = D3D12_RESOURCE_FLAG_NONE
		};

		m_Graphics->GetDevice()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_VertexBuffer));
		DXSetName(m_VertexBuffer);
		PositionColorVertex* vertexPtr;
		HR(m_VertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&vertexPtr)));
		vertexPtr[0] = { Vector3F(0, 0.5f, 0), NamedColors::Red };
		vertexPtr[1] = { Vector3F(0.5f, -0.5f, 0), NamedColors::Green };
		vertexPtr[2] = { Vector3F(-0.5f, -0.5f, 0), NamedColors::Blue };

		resourceDesc.Width = sizeof(uint32) * 3;
		m_Graphics->GetDevice()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_IndexBuffer));
		DXSetName(m_IndexBuffer);
		uint32* indexPtr;
		HR(m_IndexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&indexPtr)));
		indexPtr[0] = 0;
		indexPtr[1] = 1;
		indexPtr[2] = 2;
	}

	D3D12CommandBuffer::~D3D12CommandBuffer() noexcept
	{
	}

	void D3D12CommandBuffer::Dispose() noexcept
	{
		m_VertexBuffer.Reset();
		m_IndexBuffer.Reset();
		m_CommandPool.Reset();
		m_CommandBuffer.Reset();
	}

	void D3D12CommandBuffer::BeginCommands_Implementation()
	{
		HR(m_CommandPool->Reset());
		HR(m_CommandBuffer->Reset(m_CommandPool.Get(), nullptr));
	}

	void D3D12CommandBuffer::EndCommands_Implementation()
	{
		HR(m_CommandBuffer->Close());

		ID3D12CommandList* targetCommandBuffer = m_CommandBuffer.Get();
		auto& queue = m_Graphics->GetCommandQueue();
		queue.GetQueue()->ExecuteCommandLists(1, &targetCommandBuffer);
		m_FenceValue = queue.Signal();
	}

	void D3D12CommandBuffer::BeginRenderPass(RenderTexture* renderTexture)
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
					.StateBefore = D3D12_RESOURCE_STATE_PRESENT,
					.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET
				}
			};
			m_CommandBuffer->ResourceBarrier(1, &barrier);

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

			m_CommandBuffer->BeginRenderPass(1, &renderTargetDesc, &depthStencilDesc, D3D12_RENDER_PASS_FLAG_NONE);

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
			m_CommandBuffer->RSSetViewports(1, &viewport);

			D3D12_RECT scissorRect =
			{
				.left = 0,
				.top = 0,
				.right = static_cast<LONG>(size.X),
				.bottom = static_cast<LONG>(size.Y)
			};
			m_CommandBuffer->RSSetScissorRects(1, &scissorRect);
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

			m_CommandBuffer->EndRenderPass();
			m_CommandBuffer->ResourceBarrier(1, &barrier);
		}
	}

	void D3D12CommandBuffer::SetRenderPipeline(RenderPipeline* renderPipeline)
	{
		if (auto* pso1 = dynamic_cast<D3D12RaytracingRenderPipeline*>(renderPipeline))
		{
			m_CommandBuffer->SetPipelineState1(pso1->GetPipelineStateObject());
			return;
		}
		else if (auto* ps = dynamic_cast<D3D12GeometryRenderPipeline*>(renderPipeline))
		{
			m_CommandBuffer->SetGraphicsRootSignature(ps->GetRootSignature());
			m_CommandBuffer->SetPipelineState(ps->GetPipelineState());
			return;
		}
		else
		{
			throw InvalidOperationException(TEXT("Unsupported render pipeline type."));
		}
	}

	void D3D12CommandBuffer::Draw()
	{
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView =
		{
			.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress(),
			.SizeInBytes = sizeof(PositionColorVertex) * 3,
			.StrideInBytes = sizeof(PositionColorVertex)
		};

		D3D12_INDEX_BUFFER_VIEW indexBufferView =
		{
			.BufferLocation = m_IndexBuffer->GetGPUVirtualAddress(),
			.SizeInBytes = sizeof(uint32) * 3,
			.Format = DXGI_FORMAT_R32_UINT
		};

		m_CommandBuffer->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandBuffer->IASetVertexBuffers(0, 1, &vertexBufferView);
		m_CommandBuffer->IASetIndexBuffer(&indexBufferView);
		m_CommandBuffer->DrawInstanced(3, 1, 0, 0);
	}

	void D3D12CommandBuffer::WaitForCompletion(const TimeSpan& timeout)
	{
		auto& queue = m_Graphics->GetCommandQueue();
		queue.WaitForCompletion(m_FenceValue, timeout);
	}
}