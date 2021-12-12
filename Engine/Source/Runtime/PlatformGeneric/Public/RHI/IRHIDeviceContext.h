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
	virtual void Begin() = 0;
	virtual void End() = 0;
	virtual void SetDescriptorHeaps(int32 MaxSRVCount, int32 MaxSamplerCount) = 0;
	virtual void DrawIndexedInstanced(uint32 IndexCountPerInstance, uint32 InstanceCount, uint32 StartIndexLocation, int32 BaseVertexLocation, uint32 StartInstanceLocation) = 0;
	virtual void DrawInstanced(uint32 VertexCountPerInstance, uint32 InstanceCount, uint32 BaseVertexLocation, uint32 StartInstanceLocation) = 0;
	virtual void IASetPrimitiveTopology(ERHIPrimitiveTopology Topology) = 0;
	virtual void OMSetRenderTargets(std::shared_ptr<IRHIRenderTargetView> RTV, int32 IndexOfRTV, int32 Count, std::shared_ptr<IRHIDepthStencilView> DSV, int32 IndexOfDSV) = 0;
	virtual void ClearRenderTargetView(std::shared_ptr<IRHIRenderTargetView> RTV, int32 IndexOf, const Color& Color) = 0;
	virtual void ClearDepthStencilView(std::shared_ptr<IRHIDepthStencilView> DSV, int32 IndexOf, std::optional<float> Depth = 1.0f, std::optional<uint8> Stencil = 0) = 0;
	virtual void RSSetScissorRects(std::span<const RHIScissorRect> ScissorRects) = 0;
	virtual void RSSetViewports(std::span<const RHIViewport> Viewports) = 0;
	virtual void ResourceBarrier(std::span<const RHIResourceBarrier> Barriers) = 0;
	virtual void IASetVertexBuffers(uint32 SlotIndex, std::span<const RHIVertexBufferView> Views) = 0;
	virtual void IASetIndexBuffer(const RHIIndexBufferView& View) = 0;
	virtual void SetGraphicsShader(std::shared_ptr<IRHIShader> Shader) = 0;
	virtual void SetGraphicsRootConstantBufferView(uint32 Index, uint64 BufferLocation) = 0;
	virtual void SetGraphicsRoot32BitConstants(uint32 Index, uint32 Num32BitsToSet, const void* SrcData, uint32 DestOffsetIn32BitValues) = 0;
	virtual void SetGraphicsRootShaderResourceView(uint32 Index, uint64 BufferLocation) = 0;
	virtual void SetGraphicsRootShaderResourceView(uint32 Index, IRHIShaderResourceView* View, int32 IndexOf, int32 Count) = 0;
	virtual void PendingGarbageObject(std::shared_ptr<IRHIFactoryChild> Resource) = 0;
	virtual void UpdateSubresource(std::shared_ptr<IRHIResource> Resource, uint32 Subresource, const RHISubresourceData& Data) = 0;
	virtual void CopyResource(std::shared_ptr<IRHIResource> DstResource, std::shared_ptr<IRHIResource> SrcResource) = 0;
	virtual void ResolveSubresource(std::shared_ptr<IRHIResource> DstResource, uint32 DstSubresource, std::shared_ptr<IRHIResource> SrcResource, uint32 SrcSubresource, ERHIPixelFormat Format) = 0;
	virtual uint64 ExecuteCommandLists(std::span<std::shared_ptr<IRHIDeviceContext>> DeviceContexts, bool bSignal = false) = 0;

	void RSSetScissorRect(const RHIScissorRect& ScissorRect) { RSSetScissorRects(std::span<const RHIScissorRect>(&ScissorRect, 1)); }
	void RSSetScissorRects(std::span<RHIScissorRect> ScissorRects) { RSSetScissorRects((std::span<const RHIScissorRect>)ScissorRects); }
	void RSSetViewport(const RHIViewport& Viewport) { RSSetViewports(std::span<const RHIViewport>(&Viewport, 1)); }
	void RSSetViewports(std::span<RHIViewport> Viewports) { RSSetViewports((std::span<const RHIViewport>)Viewports); }
	void ResourceBarrier(std::span<RHIResourceBarrier> Barriers) { ResourceBarrier((std::span<const RHIResourceBarrier>)Barriers); }
	void IASetVertexBuffers(uint32 SlotIndex, std::span<RHIVertexBufferView> Views) { IASetVertexBuffers(SlotIndex, (std::span<const RHIVertexBufferView>)Views); }

	template<std::convertible_to<RHIResourceBarrier>... TBarriers>
	void ResourceBarrier(TBarriers&&... Barriers)
	{
		ResourceBarrier(std::array<RHIResourceBarrier, sizeof...(TBarriers)>{ std::forward<TBarriers>(Barriers)... });
	}

	template<std::convertible_to<RHIResourceBarrier> TBarrier>
	void ResourceBarrier(std::span<TBarrier> Barriers_span)
	{
		std::vector<RHIResourceBarrier> Barriers(Barriers_span.size());
		for (size_t i = 0; i < Barriers.size(); ++i)
		{
			Barriers[i] = Barriers_span[i];
		}
		ResourceBarrier(std::span<const RHIResourceBarrier>(Barriers));
	}

	template<class T>
	void SetGraphicsRoot32BitConstants(uint32 Index, T&& Value, uint32 DestOffsetIn32BitValues)
	{
		uint32 Num32BitsToSet = (sizeof(Value) - 1) / 4 + 1;
		SetGraphicsRoot32BitConstants(Index, Num32BitsToSet, &Value, DestOffsetIn32BitValues);
	}

	void ExecuteCommandList(std::shared_ptr<IRHIDeviceContext> DeviceContext)
	{
		std::shared_ptr<IRHIDeviceContext> DeviceContexts[] = { DeviceContext };
		ExecuteCommandLists(DeviceContexts);
	}
};