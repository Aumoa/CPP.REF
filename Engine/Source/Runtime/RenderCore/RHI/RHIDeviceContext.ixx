// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.RenderCore:RHIDeviceContext;

import :RHIDeviceChild;
import :RHICommandType;
import :ComPtr;

export class RHIDevice;

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

public /*internal*/:
	ID3D12CommandList* GetCommandList() const { return _commandList.Get(); }

protected:
	/// <summary>
	/// Swap the allocator.
	/// </summary>
	void SwapAllocator(ComPtr<ID3D12CommandAllocator>&& swap);
};