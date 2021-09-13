// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHIDeviceChild.h"
#include "ComPtr.h"

struct ID3D12Resource;

/// <summary>
/// Represents render resources.
/// </summary>
class RENDERCORE_API SRHIResource : public SRHIDeviceChild
{
	GENERATED_BODY(SRHIResource)

private:
	ComPtr<ID3D12Resource> _resource;
	void* _ptr = nullptr;

public:
	SRHIResource(SRHIDevice* device, ID3D12Resource* resource);
	~SRHIResource() override;

	/// <summary>
	/// Get GPU virtual address.
	/// </summary>
	virtual uint64 GetGPUVirtualAddress() const;

	/// <summary>
	/// Get buffer total size in bytes.
	/// </summary>
	virtual uint64 GetBufferSize() const;

	virtual void* GetMappingPointer() const;

public /*internal*/:
	ID3D12Resource* GetResource() const { return _resource.Get(); }

private:
	void Mapping();
};