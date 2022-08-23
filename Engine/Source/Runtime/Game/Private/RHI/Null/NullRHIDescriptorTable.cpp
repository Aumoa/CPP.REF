// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Null/NullRHIDescriptorTable.h"

NullRHIDescriptorTable::NullRHIDescriptorTable(std::shared_ptr<RHIDevice> device, size_t descriptors)
	: RHIDescriptorTable(std::move(device))
	, _descriptors(descriptors)
{
}

NullRHIDescriptorTable::~NullRHIDescriptorTable() noexcept
{
}

size_t NullRHIDescriptorTable::GetMaxDescriptors() const
{
	return _descriptors;
}

uint64 NullRHIDescriptorTable::CopyFrom(size_t index, RHIShaderResourceViewTable* viewTable, size_t viewIndex, size_t viewCount)
{
	return index;
}

uint64 NullRHIDescriptorTable::GetDescriptor(size_t index) const
{
	return index;
}