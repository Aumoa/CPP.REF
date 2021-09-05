// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHIDeviceChild.h"
#include "ComPtr.h"

struct ID3D12DescriptorHeap;
class RHIDeviceContext;
class RHIShaderResourceView;
struct D3D12_CPU_DESCRIPTOR_HANDLE;
struct D3D12_GPU_DESCRIPTOR_HANDLE;

class RENDERCORE_API RHIShaderDescriptorView : public RHIDeviceChild
{
	GENERATED_BODY(RHIShaderDescriptorView)

	// For access to internal, and collecting garbages.
	friend class RHIDeviceContext;

private:
	size_t _count = 0;
	uint32 _incrementSize = 0;
	ComPtr<ID3D12DescriptorHeap> _descriptorHeap;
	ComPtr<ID3D12DescriptorHeap> _pendingDestroyHeap;
	size_t _seekpos = 0;

	bool _bIsCommit : 1 = false;

public:
	/// <summary>
	/// Initialize new <see cref="RHIShaderDescriptorView"/> instance.
	/// </summary>
	/// <param name="device"> The logical device. </param>
	RHIShaderDescriptorView(RHIDevice* device);
	~RHIShaderDescriptorView() override;

	void SetMaxDescriptorCount(size_t count);
	size_t GetMaxDescriptorCount() const;

	void ResetBindings();
	size_t Bind(RHIShaderResourceView* view);

private:
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(size_t index) const;
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(size_t index) const;
};