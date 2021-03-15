// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "DirectXMinimal.h"

class DirectXCommandQueue;
class DirectXDynamicBufferAllocator;

class GAME_API DirectXDeviceBundle : virtual public Object
{
public:
	using Super = Object;

private:
	TComPtr<IDXGIFactory2> dxgiFactory;
	TComPtr<ID3D12Device5> device;
	std::map<uint64, TRefPtr<DirectXDynamicBufferAllocator>> dynamicBufferAllocators;

public:
	DirectXDeviceBundle();
	~DirectXDeviceBundle() override;

	IDXGIFactory2* GetFactory() const;
	ID3D12Device5* GetDevice() const;
	DirectXDynamicBufferAllocator* GetOrCreateDynamicBufferAllocator(uint64 allocateUnit);
	DirectXDynamicBufferAllocator* GetDynamicBufferAllocator(uint64 allocateUnit) const;

	TComPtr<ID3D12Resource> CreateImmutableBuffer(DirectXCommandQueue* commandQueue, D3D12_RESOURCE_STATES initialState, const uint8* initialBuffer, size_t sizeInBytes, D3D12_RESOURCE_FLAGS flags);
	TComPtr<ID3D12Resource> CreateDynamicBuffer(size_t sizeInBytes);

private:
	bool IsAdapterSuitable(IDXGIAdapter1* adapter) const;
	bool IsDeviceSuitable(ID3D12Device5* device) const;
};