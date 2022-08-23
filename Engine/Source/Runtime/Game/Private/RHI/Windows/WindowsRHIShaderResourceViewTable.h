// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIShaderResourceViewTable.h"

#if PLATFORM_WINDOWS

#include "WindowsRHICommon.h"

class WindowsRHIDevice;

class WindowsRHIShaderResourceViewTable : public RHIShaderResourceViewTable
{
	friend WindowsRHIDevice;

private:
	ComPtr<ID3D12DescriptorHeap> _descriptorHeap;
	uint32 _incrementSize = 0;

private:
	WindowsRHIShaderResourceViewTable(std::shared_ptr<WindowsRHIDevice> device, ERHIShaderResourceViewType type, size_t numViews);

public:
	virtual ~WindowsRHIShaderResourceViewTable() noexcept override;

	virtual void CreateUnorderedAccessView(size_t viewIndex, std::shared_ptr<RHIResource> pResource, std::shared_ptr<RHIResource> pCounter, const RHIUnorderedAccessViewDesc& desc) override;

public:
	DECLGET(Heap, _descriptorHeap);
};

inline ID3D12DescriptorHeap* WinGetr(RHIShaderResourceViewTable* heap)
{
	if (heap)
	{
		return static_cast<WindowsRHIShaderResourceViewTable*>(heap)->GetHeap().Get();
	}
	else
	{
		return nullptr;
	}
}

#endif