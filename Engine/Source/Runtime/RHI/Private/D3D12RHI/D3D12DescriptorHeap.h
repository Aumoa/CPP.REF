// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDescriptorHeap.h"

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12Common.h"

class ND3D12DescriptorHeap : public NRHIDescriptorHeap
{
private:
	ID3D12Device* pDevice = nullptr;
	bool bDirty = false;
	size_t Capacity = 0;
	size_t Count = 0;

	ComPtr<ID3D12DescriptorHeap> pShaderVisibleHeap;
	size_t IncrementSize = 0;

public:
	ND3D12DescriptorHeap(ID3D12Device& InDevice);

	virtual void Clear(bool bShrink) override;
	virtual void Reserve(size_t InCount) override;
	virtual size_t GetCount() const override { return Count; }
	virtual size_t GetCapacity() const override { return Capacity; }

	virtual void ApplyViewSimple(size_t Index, NRHIShaderResourceView& SourceView, size_t SourceIndex, size_t SourceRange) override;
	virtual int64 GetVirtualHandleLocation(size_t Index) const override;

	inline ID3D12DescriptorHeap* Get() const { return pShaderVisibleHeap.Get(); }

private:
	void AllocateShaderVisibleHeap();
};

#endif