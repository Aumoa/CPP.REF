// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.RenderCore:RHIDeviceContext;

import :RHIDeviceChild;
import :RHIEnums;
import :ComPtr;
import :RHIStructures;

export class RHIDevice;
export class RHIShader;
export class RHIRenderTargetView;

/// <summary>
/// Represents a device context which generates rendering commands.
/// </summary>
export class RHIDeviceContext : public RHIDeviceChild
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
	~RHIDeviceContext();

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
	virtual void OMSetRenderTargets(RHIRenderTargetView* rtv, int32 index, int32 count);

	/// <summary>
	/// Clear render target view as color.
	/// </summary>
	virtual void ClearRenderTargetView(RHIRenderTargetView* rtv, const Color& color);

	/// <summary>
	/// Clear render target view as color.
	/// </summary>
	virtual void ClearRenderTargetView(RHIRenderTargetView* rtv, int32 index, const Color& color);

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

public /*internal*/:
	ID3D12CommandList* GetCommandList() const { return _commandList.Get(); }

protected:
	/// <summary>
	/// Swap the allocator.
	/// </summary>
	void SwapAllocator(ComPtr<ID3D12CommandAllocator>&& swap);
};