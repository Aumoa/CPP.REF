// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Null/NullRHIShaderBindingTable.h"
#include "RHI/RHIStructures.h"

NullRHIShaderBindingTable::NullRHIShaderBindingTable(std::shared_ptr<RHIDevice> device)
	: RHIShaderBindingTable(std::move(device))
{
}

NullRHIShaderBindingTable::~NullRHIShaderBindingTable() noexcept
{
}

RHIGPUVirtualAddressRange NullRHIShaderBindingTable::GetRayGenerationShaderRecord(const void* shaderIdentifier) const
{
	return {};
}

RHIGPUVirtualAddressRangeAndStride NullRHIShaderBindingTable::GetMissShaderTable() const
{
	return {};
}

RHIGPUVirtualAddressRangeAndStride NullRHIShaderBindingTable::GetHitGroupTable() const
{
	return {};
}

RHIGPUVirtualAddressRangeAndStride NullRHIShaderBindingTable::GetCallableShaderTable() const
{
	return {};
}