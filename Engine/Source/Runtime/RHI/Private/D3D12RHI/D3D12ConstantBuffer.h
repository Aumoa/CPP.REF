// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIConstantBuffer.h"

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12Common.h"

class ND3D12ConstantBuffer : public NRHIConstantBuffer
{
	ID3D12Device* pDevice = nullptr;
	ComPtr<ID3D12Resource> pBuffer;
	size_t BufferSize = 0;
	void* pMapped = nullptr;

public:
	ND3D12ConstantBuffer(ID3D12Device& InDevice);

	virtual void Reserve(size_t InBufferSize) override;
	virtual size_t GetBufferSize() const override { return BufferSize; }
	virtual void* GetBufferPointer() const override { return pMapped; }
	virtual int64 GetGPUVirtualAddress() const override { return (int64)pBuffer->GetGPUVirtualAddress(); }
};

#endif