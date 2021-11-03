// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12DeviceChild.h"
#include "D3D12Resource.h"
#include "DirectXCommon.h"
#include "RHI/IRHIDeviceContext.h"

class SD3D12DescriptorHeap;

class SD3D12CommandList : public SD3D12DeviceChild, implements IRHIDeviceContext
{
	GENERATED_BODY(SD3D12CommandList)

private:
	ComPtr<ID3D12GraphicsCommandList> CommandList;
	std::vector<SObject*> PendingObjects;

	SD3D12DescriptorHeap* HeapForSRV = nullptr;
	SD3D12DescriptorHeap* HeapForSampler = nullptr;

public:
	SD3D12CommandList(SDXGIFactory* factory, SD3D12Device* device);

	virtual void Begin() override;
	virtual void End() override;
	virtual void SetDescriptorHeaps(int32 MaxSRVCount, int32 MaxSamplerCount) override;
	virtual void DrawIndexedInstanced(uint32 indexCountPerInstance, uint32 instanceCount, uint32 startIndexLocation, int32 baseVertexLocation, uint32 startInstanceLocation) override;
	virtual void DrawInstanced(uint32 vertexCountPerInstance, uint32 instanceCount, uint32 baseVertexLocation, uint32 startInstanceLocation) override;
	virtual void IASetPrimitiveTopology(ERHIPrimitiveTopology topology) override;
	virtual void OMSetRenderTargets(IRHIRenderTargetView* rtv, int32 indexOfRTV, int32 count, IRHIDepthStencilView* dsv, int32 indexOfDSV) override;
	virtual void ClearRenderTargetView(IRHIRenderTargetView* rtv, int32 indexOf, const Color& color) override;
	virtual void ClearDepthStencilView(IRHIDepthStencilView* dsv, int32 indexOf, std::optional<float> depth, std::optional<uint8> stencil) override;
	virtual void RSSetScissorRects(std::span<const RHIScissorRect> scissorRects) override;
	virtual void RSSetViewports(std::span<const RHIViewport> viewports) override;
	virtual void ResourceBarrier(std::span<const RHIResourceBarrier> barriers) override;
	virtual void IASetVertexBuffers(uint32 slotIndex, std::span<const RHIVertexBufferView> views) override;
	virtual void IASetIndexBuffer(const RHIIndexBufferView& view) override;
	virtual void SetGraphicsShader(IRHIShader* shader) override;
	virtual void SetGraphicsRootConstantBufferView(uint32 index, uint64 bufferLocation) override;
	virtual void SetGraphicsRoot32BitConstants(uint32 index, uint32 num32BitsToSet, const void* srcData, uint32 destOffsetIn32BitValues) override;
	virtual void SetGraphicsRootShaderResourceView(uint32 index, uint64 bufferLocation) override;
	virtual void SetGraphicsRootShaderResourceView(uint32 index, IRHIShaderResourceView* view, int32 indexOf, int32 count) override;
	virtual void PendingGarbageObject(SObject* object) override;
	virtual void UpdateSubresource(IRHIResource* resource, uint32 subresource, const RHISubresourceData& data) override;
	virtual void CopyResource(IRHIResource* DstResource, IRHIResource* SrcResource) override;
	virtual uint64 ExecuteCommandLists(std::span<IRHIDeviceContext*> deviceContexts, bool bSignal) override;

	virtual void CopyTextureRegion(const D3D12_TEXTURE_COPY_LOCATION* pDst, UINT DstX, UINT DstY, UINT DstZ, const D3D12_TEXTURE_COPY_LOCATION* pSrc, const D3D12_BOX* pSrcBox);
	virtual void CopyResource(ID3D12Resource* dst, ID3D12Resource* src);
	virtual void ResourceBarrier(UINT NumBarriers, const D3D12_RESOURCE_BARRIER* pBarriers);

	std::vector<SObject*> ClearPendingObjects();

public:
	DECLARE_GETTER(ID3D12GraphicsCommandList, CommandList);
	DECLARE_GETTER(ID3D12CommandList, CommandList);

private:
	template<class T>
	const T& Conv(const T& value) { return value; }
	auto Conv(ERHIPrimitiveTopology value) { return static_cast<D3D12_PRIMITIVE_TOPOLOGY>(value); }
	auto Conv(IRHIResource* value) { return Cast<SD3D12Resource>(value)->Get<ID3D12Resource>(); }
	auto Conv(ERHIResourceStates value) { return static_cast<D3D12_RESOURCE_STATES>(value); }
	auto Conv(const RHIVertexBufferView& view) { return reinterpret_cast<const D3D12_VERTEX_BUFFER_VIEW&>(view); }
	auto Conv(const RHIIndexBufferView& view) { return reinterpret_cast<const D3D12_INDEX_BUFFER_VIEW*>(&view); }

	D3D12_RESOURCE_BARRIER Conv(const RHIResourceBarrier& value)
	{
		D3D12_RESOURCE_BARRIER barrier = {};
		switch (value.index())
		{
		case 0:
		{
			const auto& transition = std::get<RHIResourceTransitionBarrier>(value);
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Transition.pResource = Conv(transition.pResource);
			barrier.Transition.StateBefore = Conv(transition.StateBefore);
			barrier.Transition.StateAfter = Conv(transition.StateAfter);
			barrier.Transition.Subresource = Conv(transition.Subresource);
			break;
		}
		case 1:
		{
			const auto& aliasing = std::get<RHIResourceAliasingBarrier>(value);
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_ALIASING;
			barrier.Aliasing.pResourceBefore = Conv(aliasing.pResourceBefore);
			barrier.Aliasing.pResourceAfter = Conv(aliasing.pResourceAfter);
			break;
		}
		case 2:
		{
			const auto& uav = std::get<RHIResourceUAVBarrier>(value);
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
			barrier.UAV.pResource = Conv(uav.pResource);
			break;
		}
		}
		return barrier;
	}

	template<class T>
	auto Conv(std::span<const T> values)
	{
		using ConvT = std::remove_reference_t<std::remove_const_t<decltype(Conv(std::declval<T>()))>>;
		std::vector<ConvT> values_conv(values.size());
		for (size_t i = 0; i < values.size(); ++i)
		{
			values_conv[i] = Conv(values[i]);
		}
		return values_conv;
	}
};