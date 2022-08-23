// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDescriptorTable.h"

#if PLATFORM_WINDOWS

#include "RHI/Windows/WindowsRHICommon.h"

class WindowsRHIDevice;

class WindowsRHIDescriptorTable : public RHIDescriptorTable
{
	friend WindowsRHIDevice;

private:
	ComPtr<ID3D12DescriptorHeap> _heap;
	size_t _descriptors;
	size_t _incr;
	ID3D12Device* _dev;

private:
	WindowsRHIDescriptorTable(std::shared_ptr<WindowsRHIDevice> device, size_t descriptors);

public:
	virtual ~WindowsRHIDescriptorTable() noexcept override;

	virtual size_t GetMaxDescriptors() const override;
	virtual uint64 CopyFrom(size_t index, RHIShaderResourceViewTable* viewTable, size_t viewIndex, size_t viewCount) override;
	virtual uint64 GetDescriptor(size_t index) const override;

public:
	DECLGET(Heap, _heap);
};

inline ID3D12DescriptorHeap* WinGetr(RHIDescriptorTable* heap)
{
	if (heap)
	{
		return static_cast<WindowsRHIDescriptorTable*>(heap)->GetHeap().Get();
	}
	else
	{
		return nullptr;
	}
}

#endif