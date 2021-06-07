// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.RenderCore:RHIDeviceContext;

import :RHIDeviceChild;
import :RHIEnums;
import :ComPtr;

export class RHIDevice;
export class RHIShader;

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

public /*internal*/:
	ID3D12CommandList* GetCommandList() const { return _commandList.Get(); }

protected:
	/// <summary>
	/// Swap the allocator.
	/// </summary>
	void SwapAllocator(ComPtr<ID3D12CommandAllocator>&& swap);
};