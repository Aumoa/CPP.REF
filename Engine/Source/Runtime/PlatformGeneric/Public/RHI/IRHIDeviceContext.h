// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IRHIDeviceChild.h"
#include "RHIStructures.h"
#include <variant>

interface IRHIShader;
interface IRHIRenderTargetView;
interface IRHIDepthStencilView;
interface IRHIShaderResourceView;

interface IRHIDeviceContext : implements IRHIDeviceChild
{
	GENERATED_INTERFACE_BODY(IRHIDeviceContext)

	virtual void Begin() = 0;
	virtual void End() = 0;
	virtual void SetDescriptorHeaps(int32 MaxSRVCount, int32 MaxSamplerCount) = 0;
	virtual void DrawIndexedInstanced(uint32 indexCountPerInstance, uint32 instanceCount, uint32 startIndexLocation, int32 baseVertexLocation, uint32 startInstanceLocation) = 0;
	virtual void DrawInstanced(uint32 vertexCountPerInstance, uint32 instanceCount, uint32 baseVertexLocation, uint32 startInstanceLocation) = 0;
	virtual void IASetPrimitiveTopology(ERHIPrimitiveTopology topology) = 0;
	virtual void OMSetRenderTargets(IRHIRenderTargetView* rtv, int32 indexOfRTV, int32 count, IRHIDepthStencilView* dsv, int32 indexOfDSV) = 0;
	virtual void ClearRenderTargetView(IRHIRenderTargetView* rtv, int32 indexOf, const Color& color) = 0;
	virtual void ClearDepthStencilView(IRHIDepthStencilView* dsv, int32 indexOf, std::optional<float> depth, std::optional<uint8> stencil) = 0;
	virtual void RSSetScissorRects(std::span<const RHIScissorRect> scissorRects) = 0;
	virtual void RSSetViewports(std::span<const RHIViewport> viewports) = 0;
	virtual void ResourceBarrier(std::span<const RHIResourceBarrier> barriers) = 0;
	virtual void IASetVertexBuffers(uint32 slotIndex, std::span<const RHIVertexBufferView> views) = 0;
	virtual void IASetIndexBuffer(const RHIIndexBufferView& view) = 0;
	virtual void SetGraphicsShader(IRHIShader* shader) = 0;
	virtual void SetGraphicsRootConstantBufferView(uint32 index, uint64 bufferLocation) = 0;
	virtual void SetGraphicsRoot32BitConstants(uint32 index, uint32 num32BitsToSet, const void* srcData, uint32 destOffsetIn32BitValues) = 0;
	virtual void SetGraphicsRootShaderResourceView(uint32 index, uint64 bufferLocation) = 0;
	virtual void SetGraphicsRootShaderResourceView(uint32 index, IRHIShaderResourceView* view, int32 indexOf, int32 count) = 0;
	virtual void PendingGarbageObject(SObject* object) = 0;
	virtual void UpdateSubresource(IRHIResource* resource, uint32 subresource, const RHISubresourceData& data) = 0;
	virtual void CopyResource(IRHIResource* DstResource, IRHIResource* SrcResource) = 0;
	virtual void ResolveSubresource(IRHIResource* DstResource, uint32 DstSubresource, IRHIResource* SrcResource, uint32 SrcSubresource, ERHIPixelFormat Format) = 0;
	virtual uint64 ExecuteCommandLists(std::span<IRHIDeviceContext*> deviceContexts, bool bSignal = false) = 0;

	void RSSetScissorRect(const RHIScissorRect& scissorRect) { RSSetScissorRects(std::span<const RHIScissorRect>(&scissorRect, 1)); }
	void RSSetScissorRects(std::span<RHIScissorRect> scissorRects) { RSSetScissorRects((std::span<const RHIScissorRect>)scissorRects); }
	void RSSetViewport(const RHIViewport& viewport) { RSSetViewports(std::span<const RHIViewport>(&viewport, 1)); }
	void RSSetViewports(std::span<RHIViewport> viewports) { RSSetViewports((std::span<const RHIViewport>)viewports); }
	void ResourceBarrier(std::span<RHIResourceBarrier> barriers) { ResourceBarrier((std::span<const RHIResourceBarrier>)barriers); }
	void IASetVertexBuffers(uint32 slotIndex, std::span<RHIVertexBufferView> views) { IASetVertexBuffers(slotIndex, (std::span<const RHIVertexBufferView>)views); }

	template<std::convertible_to<RHIResourceBarrier>... TBarriers>
	void ResourceBarrier(TBarriers&&... barriers)
	{
		ResourceBarrier(std::array<RHIResourceBarrier, sizeof...(TBarriers)>{ std::forward<TBarriers>(barriers)... });
	}

	template<std::convertible_to<RHIResourceBarrier> TBarrier>
	void ResourceBarrier(std::span<TBarrier> barriers_span)
	{
		std::vector<RHIResourceBarrier> barriers(barriers_span.size());
		for (size_t i = 0; i < barriers.size(); ++i)
		{
			barriers[i] = barriers_span[i];
		}
		ResourceBarrier(std::span<const RHIResourceBarrier>(barriers));
	}

	template<class T>
	void SetGraphicsRoot32BitConstants(uint32 index, T&& value, uint32 destOffsetIn32BitValues)
	{
		uint32 num32BitsToSet = (sizeof(value) - 1) / 4 + 1;
		SetGraphicsRoot32BitConstants(index, num32BitsToSet, &value, destOffsetIn32BitValues);
	}

	void ExecuteCommandList(IRHIDeviceContext* deviceContext)
	{
		IRHIDeviceContext* deviceContexts[] = { deviceContext };
		ExecuteCommandLists(deviceContexts);
	}
};