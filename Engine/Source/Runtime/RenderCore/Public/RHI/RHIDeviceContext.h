// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHIDeviceChild.h"
#include "ComPtr.h"
#include "RHIEnums.h"
#include "RHIStructures.h"

struct ID3D12CommandAllocator;
struct ID3D12GraphicsCommandList;
struct ID3D12CommandList;
class RHIShader;
class RHIRenderTargetView;
class RHIDepthStencilView;

/// <summary>
/// Represents a device context which generates rendering commands.
/// </summary>
class RENDERCORE_API RHIDeviceContext : public RHIDeviceChild
{
public:
	using Super = RHIDeviceChild;

private:
	const ERHICommandType _type;
	ComPtr<ID3D12CommandAllocator> _allocator;
	ComPtr<ID3D12GraphicsCommandList> _commandList;

public:
	/// <summary>
	/// Initialize new <see cref="RHIDeviceContext"/> instance.
	/// </summary>
	/// <param name="device"> The logical device. </param>
	/// <param name="commandType"> Specify command type for usage. </param>
	RHIDeviceContext(RHIDevice* device, ERHICommandType commandType = ERHICommandType::Direct);

	/// <summary>
	/// Mark the beginning of a series of commands.
	/// </summary>
	virtual void Begin();

	/// <summary>
	/// Mark the end of a series of commands.
	/// </summary>
	virtual void End();

	/// <summary>
	/// Draw primitive with vertex buffer and index buffer.
	/// </summary>
	virtual void DrawIndexedInstanced(uint32 indexCountPerInstance, uint32 instanceCount = 1, uint32 startIndexLocation = 0, int32 baseVertexLocation = 0, uint32 startInstanceLocation = 0);

	/// <summary>
	/// Draw primitive with vertex buffer.
	/// </summary>
	virtual void DrawInstanced(uint32 vertexCountPerInstance, uint32 instanceCount = 1, int32 baseVertexLocation = 0, uint32 startInstanceLocation = 0);

	/// <summary>
	/// Set IA primitive topology.
	/// </summary>
	virtual void IASetPrimitiveTopology(ERHIPrimitiveTopology topology);

	/// <summary>
	/// Set graphics pipeline shader program.
	/// </summary>
	virtual void SetGraphicsShader(RHIShader* shader);

	/// <summary>
	/// Set render target view.
	/// </summary>
	virtual void OMSetRenderTargets(RHIRenderTargetView* rtv);

	/// <summary>
	/// Set render target view.
	/// </summary>
	virtual void OMSetRenderTargets(RHIRenderTargetView* rtv, int32 rtvStart, int32 count, RHIDepthStencilView* dsv, int32 dsvStart);

	/// <summary>
	/// Clear render target view as color.
	/// </summary>
	virtual void ClearRenderTargetView(RHIRenderTargetView* rtv, int32 index, const Color& color);

	/// <summary>
	/// Clear render target view as color.
	/// </summary>
	virtual void ClearDepthStencilView(RHIDepthStencilView* dsv, int32 index, std::optional<float> depth, std::optional<uint8> stencil);

	/// <summary>
	/// Set scissor rects.
	/// </summary>
	virtual void RSSetScissorRects(int32 count, const RHIScissorRect* rects);

	/// <summary>
	/// Set viewports.
	/// </summary>
	virtual void RSSetViewports(int32 count, const RHIViewport* viewports);

	/// <summary>
	/// Set transition barriers.
	/// </summary>
	virtual void TransitionBarrier(int32 count, const RHITransitionBarrier* barriers);

	/// <summary>
	/// Set vertex buffer view to IA slot.
	/// </summary>
	virtual void IASetVertexBuffers(uint32 startSlot, uint32 numViews, const RHIVertexBufferView* views);

	virtual void IASetIndexBuffer(const RHIIndexBufferView& view);
	virtual void SetGraphicsRootConstantBufferView(uint32 index, uint64 bufferLocation);
	virtual void SetGraphicsRoot32BitConstants(uint32 index, uint32 num32BitsToSet, const void* srcData, uint32 destOffsetIn32BitValues);

public /*internal*/:
	ID3D12CommandList* GetCommandList() const;

protected:
	/// <summary>
	/// Swap the allocator.
	/// </summary>
	void SwapAllocator(ComPtr<ID3D12CommandAllocator>&& swap);
};