// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.RenderCore:RHIRenderTargetView;

import :RHIView;
import SC.Runtime.RenderCore.Internal;
import :ComPtr;

export class RHITexture2D;

/// <summary>
/// Represents render target view for binding to gpu output.
/// </summary>
export class RHIRenderTargetView : public RHIView
{
public:
	using Super = RHIView;

private:
	ComPtr<ID3D12DescriptorHeap> _descriptor;
	const uint32 _descriptorCount = 0;
	uint32 _increment = 0;

public:
	RHIRenderTargetView(RHIDevice* device, uint32 descriptorCount);

	/// <summary>
	/// Create render target view.
	/// </summary>
	void CreateRenderTargetView(RHITexture2D* texture, int32 index);

	/// <summary>
	/// Get initialized descriptor count.
	/// </summary>
	int32 GetDescriptorCount() const { return _descriptorCount; }

public /*internal*/:
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(int32 index = 0) const;
};