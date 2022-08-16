// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/RHIShaderResourceViewTable.h"

RHIShaderResourceViewTable::RHIShaderResourceViewTable(std::shared_ptr<RHIDevice> device)
	: RHIDeviceResource(std::move(device))
{
}

RHIShaderResourceViewTable::~RHIShaderResourceViewTable() noexcept
{
}