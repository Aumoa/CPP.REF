// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHIDeviceChild.h"
#include "ComPtr.h"
#include "RHIEnums.h"
#include "RHIStructures.h"
#include <variant>

struct IUnknown;
struct ID3D12CommandAllocator;
struct ID3D12GraphicsCommandList;
struct ID3D12CommandList;
class RHIShader;
class RHIRenderTargetView;
class RHIDepthStencilView;
class RHIShaderDescriptorView;
class RHICommandQueue;
class RHIShaderResourceView;

/// <summary>
/// Represents a device context which generates rendering commands.
/// </summary>
class RENDERCORE_API RHIDeviceContext : public RHIDeviceChild
{
	CLASS_BODY(RHIDeviceContext)

private:
	const ERHICommandType _type;
	ComPtr<ID3D12CommandAllocator> _allocator;
	ComPtr<ID3D12GraphicsCommandList> _commandList;

	RHIShaderDescriptorView* _resourceView = nullptr;

public:
	/// <summary>
	/// Initialize new <see cref="RHIDeviceContext"/> instance.
	/// </summary>
	/// <param name="device"> The logical device. </param>
	/// <param name="commandType"> Specify command type for usage. </param>
	RHIDeviceContext(RHIDevice* device, ERHICommandType commandType = ERHICommandType::Direct);
	~RHIDeviceContext() override;

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

	/// <summary>
	/// Bind an index buffer to the input-assembler stage.
	/// </summary>
	virtual void IASetIndexBuffer(const RHIIndexBufferView& view);

	/// <summary>
	/// Sets a CPU descriptor handle for the constant buffer in the graphics root signature.
	/// </summary>
	/// <param name="index"> The slot number for binding. </param>
	/// <param name="bufferLocation"> The GPU virtual address of the constant buffer. </param>
	virtual void SetGraphicsRootConstantBufferView(uint32 index, uint64 bufferLocation);

	/// <summary>
	/// Sets a group of constants in the graphics root signature
	/// </summary>
	/// <param name="index"> The slot number for binding. </param>
	/// <param name="num32BitsToSet"> The number of constants to set in the root signature. </param>
	/// <param name="srcData"> The source data for the group of constants to set. </param>
	/// <param name="destOffsetIn32BitValues"> The offset, in 32-bit values, to set the first constant of the group in the root signature. </param>
	virtual void SetGraphicsRoot32BitConstants(uint32 index, uint32 num32BitsToSet, const void* srcData, uint32 destOffsetIn32BitValues);

	/// <summary>
	/// Sets a CPU descriptor handle for the shader resource in the graphics root signature.
	/// </summary>
	/// <param name="index"> The slot number for binding. </param>
	/// <param name="bufferLocation"> The GPU virtual address of the Buffer. Textures are not supported. D3D12_GPU_VIRTUAL_ADDRESS is a typedef'd alias of UINT64. </param>
	virtual void SetGraphicsRootShaderResourceView(uint32 index, uint64 bufferLocation);

	/// <summary>
	/// Sets a GPU descriptor handle for the shader resource in the graphics root signature.
	/// </summary>
	/// <param name="index"> The slot number for binding. </param>
	/// <param name="shaderResourceView"> The resource view that contains GPU descriptor handle. </param>
	virtual void SetGraphicsRootShaderResourceView(uint32 index, RHIShaderResourceView* resourceView);

	/// <summary>
	/// Changes the currently bound descriptor views that are associated with a device context.
	/// </summary>
	/// <param name="resourceView"> A pointer to resource descriptors view. </param>
	/// <param name="samplerView"> A pointer to sampler descriptors view. </param>
	virtual void SetShaderDescriptorViews(RHIShaderDescriptorView* resourceView, RHIShaderDescriptorView* samplerView = nullptr);

private:
	std::vector<Object*> _pendingDestroyObjects;
	std::vector<IUnknown*> _pendingDestroyUnknowns;

public:
	/// <summary>
	/// Add pending destroy object. These will add to garbage object at CommandQueue object when this is commit.
	/// </summary>
	void AddPendingDestroyObject(Object* object);

	/// <summary>
	/// Add pending destroy IUnknown object. These will add to garbage object at CommandQueue object when this is commit.
	/// </summary>
	void AddPendingDestroyObject(IUnknown* unknown);

	/// <summary>
	/// Flush pending destroy objects to CommandQueue object.
	/// </summary>
	void FlushPendingDestroyObjects(uint64 fenceValue, RHICommandQueue* queue);

public /*internal*/:
	ID3D12CommandList* GetCommandList() const;

protected:
	/// <summary>
	/// Swap the allocator.
	/// </summary>
	void SwapAllocator(ComPtr<ID3D12CommandAllocator>&& swap);
};