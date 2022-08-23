// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/RHIDescriptorTable.h"

RHIDescriptorTable::RHIDescriptorTable(std::shared_ptr<RHIDevice> device)
	: RHIDeviceResource(std::move(device))
{
}

RHIDescriptorTable::~RHIDescriptorTable() noexcept
{
}