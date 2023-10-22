// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

#if PLATFORM_WINDOWS

#include "RHI/RHIStructuredBuffer.h"
#include "D3D12RHI/D3D12Common.h"

class ND3D12StructuredBuffer : public NRHIStructuredBuffer
{
	ComPtr<ID3D12Resource> pResource;
	ComPtr<ID3D12Resource> pUploadHeap;
	void* pUploadPtr;
	size_t BufferSize = 0;

public:
	ND3D12StructuredBuffer(ID3D12Device& InDevice, size_t BufferSize);

	virtual size_t GetBufferSize() const override { return BufferSize; }
	virtual void* GetBufferPointer() const override { return pUploadPtr; }
	virtual Task<> CommitAsync() override;
	virtual int64 GetGPUVirtualAddress() const override { return (int64)pResource->GetGPUVirtualAddress(); }
};

#endif