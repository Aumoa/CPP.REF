// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIShaderBindingTable.h"

class NullRHIDevice;

class NullRHIShaderBindingTable : public RHIShaderBindingTable
{
	friend NullRHIDevice;

private:
	NullRHIShaderBindingTable(std::shared_ptr<RHIDevice> device);

public:
	virtual ~NullRHIShaderBindingTable() noexcept override;

	virtual RHIGPUVirtualAddressRange GetRayGenerationShaderRecord(const void* shaderIdentifier) const override;
	virtual RHIGPUVirtualAddressRangeAndStride GetMissShaderTable() const override;
	virtual RHIGPUVirtualAddressRangeAndStride GetHitGroupTable() const override;
	virtual RHIGPUVirtualAddressRangeAndStride GetCallableShaderTable() const override;
};