// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/RHIShaderBindingTable.h"

RHIShaderBindingTable::RHIShaderBindingTable(std::shared_ptr<RHIDevice> device)
	: RHIDeviceResource(std::move(device))
{
}

RHIShaderBindingTable::~RHIShaderBindingTable() noexcept
{
}