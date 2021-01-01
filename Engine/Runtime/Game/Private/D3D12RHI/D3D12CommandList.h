// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHICommandList.h"

#include "D3D12Minimal.h"

class D3D12CommandList : virtual public Object, virtual public IRHICommandList
{
public:
	using Super = Object;

private:
	std::vector<D3D12_RESOURCE_BARRIER> pendingBarriers;
	bool bHasBegunCommand : 1;

public:
	D3D12CommandList();
	~D3D12CommandList() override;

	virtual void BeginCommand();
	virtual void EndCommand();

	virtual void SetRenderTargets(size_t count, IRHIRenderTargetView* rtv[]);
	virtual void ClearRenderTargetView(IRHIRenderTargetView* rtv);
	virtual void ResourceTransition(IRHIResource* resource, RHIResourceStates beforeState, RHIResourceStates afterState, size_t subresourceIndex = 0);
	virtual void CopyResource(IRHIResource* target, IRHIResource* source);
	virtual void SetShader(IRHIShader* shader);
	virtual void DrawInstanced(uint32 vertexCountPerInstance, uint32 instanceCount, int32 baseVertexLocation, uint32 startInstanceLocation);
	virtual void DrawIndexedInstanced(uint32 indexCountPerInstance, uint32 instanceCount, uint32 startIndexLocation, int32 baseVertexLocation, uint32 startInstanceLocation);
	virtual void SetPrimitiveTopology(RHIPrimitiveTopology primitiveTopology);
	virtual void SetScissorRects(const Rect& scissorRect);
	virtual void SetViewports(const RHIViewport& viewport);

	virtual bool HasBegunCommand_get() const;

	vs_property_get(ID3D12GraphicsCommandList*, CommandList);
	virtual ID3D12GraphicsCommandList* CommandList_get() = 0;

private:
	void ConsumePendingDeferredCommands();
};