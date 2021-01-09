// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12CommandList.h"

#include "D3D12RenderTargetView.h"
#include "D3D12Resource.h"
#include "D3D12Shader.h"
#include "RHI/RHIViewport.h"
#include "RHI/RHIMeshDrawCommand.h"

using namespace std;

D3D12CommandList::D3D12CommandList() : Super()
	, bHasBegunCommand(false)
{

}

D3D12CommandList::~D3D12CommandList()
{

}

void D3D12CommandList::BeginCommand()
{
	bHasBegunCommand = true;
}

void D3D12CommandList::EndCommand()
{
	ConsumePendingDeferredCommands();
	HR(CommandList->Close());
	bHasBegunCommand = false;
}

void D3D12CommandList::SetRenderTargets(size_t count, IRHIRenderTargetView* rtv[])
{
	ConsumePendingDeferredCommands();

	vector<D3D12_CPU_DESCRIPTOR_HANDLE> handles(count);
	for (size_t i = 0; i < count; ++i)
	{
		auto rtv_cast = dynamic_cast<D3D12RenderTargetView*>(rtv[i]);
		handles[i] = rtv_cast->Handle;
	}

	CommandList->OMSetRenderTargets((UINT)count, handles.data(), FALSE, nullptr);
}

void D3D12CommandList::ClearRenderTargetView(IRHIRenderTargetView* rtv)
{
	ConsumePendingDeferredCommands();

	constexpr FLOAT ClearColor[4] = { 0, 0, 0, 0 };
	auto rtv_cast = Cast<D3D12RenderTargetView>(rtv);
	CommandList->ClearRenderTargetView(rtv_cast->Handle, ClearColor, 0, nullptr);
}

void D3D12CommandList::ResourceTransition(IRHIResource* resource, ERHIResourceStates beforeState, ERHIResourceStates afterState, size_t subresourceIndex)
{
	D3D12_RESOURCE_BARRIER barrier{ };
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = Cast<D3D12Resource>(resource)->Resource;
	barrier.Transition.StateBefore = (D3D12_RESOURCE_STATES)beforeState;
	barrier.Transition.StateAfter = (D3D12_RESOURCE_STATES)afterState;
	barrier.Transition.Subresource = (uint32)subresourceIndex;

	pendingBarriers.emplace_back(barrier);
}

void D3D12CommandList::CopyResource(IRHIResource* target, IRHIResource* source)
{
	ConsumePendingDeferredCommands();

	CommandList->CopyResource(Cast<D3D12Resource>(target)->Resource, Cast<D3D12Resource>(source)->Resource);
}

void D3D12CommandList::SetShader(IRHIShader* shader)
{
	ConsumePendingDeferredCommands();

	auto d3d12Shader = Cast<D3D12Shader>(shader);
	CommandList->SetGraphicsRootSignature(d3d12Shader->RootSignature);
	CommandList->SetPipelineState(d3d12Shader->PipelineState);
}

void D3D12CommandList::SetPrimitiveTopology(ERHIPrimitiveTopology primitiveTopology)
{
	ConsumePendingDeferredCommands();

	CommandList->IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)primitiveTopology);
}

void D3D12CommandList::SetScissorRects(const Rect& scissorRect)
{
	ConsumePendingDeferredCommands();

	D3D12_RECT rc;
	rc.left = (INT)scissorRect.Min.X;
	rc.top = (INT)scissorRect.Min.Y;
	rc.right = (INT)scissorRect.Max.X;
	rc.bottom = (INT)scissorRect.Max.Y;

	CommandList->RSSetScissorRects(1, &rc);
}

void D3D12CommandList::SetViewports(const RHIViewport& scissorRect)
{
	ConsumePendingDeferredCommands();

	D3D12_VIEWPORT vp;
	vp.TopLeftX = (FLOAT)scissorRect.X;
	vp.TopLeftY = (FLOAT)scissorRect.Y;
	vp.Width = (FLOAT)scissorRect.Width;
	vp.Height = (FLOAT)scissorRect.Height;
	vp.MinDepth = (FLOAT)scissorRect.Near;
	vp.MaxDepth = (FLOAT)scissorRect.Far;

	CommandList->RSSetViewports(1, &vp);
}

void D3D12CommandList::DrawMesh(const RHIMeshDrawCommand& command)
{
	ConsumePendingDeferredCommands();

	D3D12_VERTEX_BUFFER_VIEW vbv = { };
	vbv.BufferLocation = command.VertexBufferVirtualAddress;
	vbv.StrideInBytes = command.VertexStride;
	vbv.SizeInBytes = vbv.StrideInBytes * command.VertexCount;
	CommandList->IASetVertexBuffers(0, 1, &vbv);

	if (command.IndexBufferVirtualAddress != 0)
	{
		D3D12_INDEX_BUFFER_VIEW ibv = { };
		ibv.BufferLocation = command.IndexBufferVirtualAddress;
		ibv.SizeInBytes = (uint32)sizeof(uint32) * command.IndexCount;
		ibv.Format = DXGI_FORMAT_R32_UINT;
		CommandList->IASetIndexBuffer(&ibv);
		CommandList->DrawIndexedInstanced(command.IndexCount, 1, 0, 0, 0);
	}
	else
	{
		CommandList->DrawInstanced(command.VertexCount, 1, 0, 0);
	}
}

bool D3D12CommandList::HasBegunCommand_get() const
{
	return bHasBegunCommand;
}

void D3D12CommandList::ConsumePendingDeferredCommands()
{
	{
		vector<D3D12_RESOURCE_BARRIER> barrier_swap;
		swap(barrier_swap, pendingBarriers);

		if (!barrier_swap.empty())
		{
			CommandList->ResourceBarrier((UINT)barrier_swap.size(), barrier_swap.data());
		}
	}
}