// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHIView.h"
#include "ComPtr.h"

struct ID3D12DescriptorHeap;
struct D3D12_CPU_DESCRIPTOR_HANDLE;
class RHITexture2D;

/// <summary>
/// Represents render target view for binding to gpu output.
/// </summary>
class RENDERCORE_API RHIRenderTargetView : public RHIView
{
	CLASS_BODY(RHIRenderTargetView)

private:
	ComPtr<ID3D12DescriptorHeap> _descriptor;
	const uint32 _descriptorCount = 0;
	uint32 _increment = 0;

public:
	RHIRenderTargetView(RHIDevice* device, uint32 descriptorCount);
	~RHIRenderTargetView() override;

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