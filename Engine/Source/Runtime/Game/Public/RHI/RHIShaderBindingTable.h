// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDeviceResource.h"

struct RHIGPUVirtualAddressRange;
struct RHIGPUVirtualAddressRangeAndStride;

class GAME_API RHIShaderBindingTable : public RHIDeviceResource
{
protected:
	RHIShaderBindingTable(std::shared_ptr<RHIDevice> device);

public:
	virtual ~RHIShaderBindingTable() noexcept override;

	virtual RHIGPUVirtualAddressRange GetRayGenerationShaderRecord(const void* shaderIdentifier) const = 0;
	virtual RHIGPUVirtualAddressRangeAndStride GetMissShaderTable() const = 0;
	virtual RHIGPUVirtualAddressRangeAndStride GetHitGroupTable() const = 0;
	virtual RHIGPUVirtualAddressRangeAndStride GetCallableShaderTable() const = 0;
};