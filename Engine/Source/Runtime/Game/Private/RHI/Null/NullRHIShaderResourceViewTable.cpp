// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Null/NullRHIShaderResourceViewTable.h"
#include "RHI/RHIEnums.h"

NullRHIShaderResourceViewTable::NullRHIShaderResourceViewTable(std::shared_ptr<RHIDevice> device, ERHIShaderResourceViewType type, size_t numViews)
	: RHIShaderResourceViewTable(std::move(device))
	, _type(type)
	, _numViews(numViews)
{
}

NullRHIShaderResourceViewTable::~NullRHIShaderResourceViewTable() noexcept
{
}

void NullRHIShaderResourceViewTable::CreateUnorderedAccessView(size_t viewIndex, std::shared_ptr<RHIResource> pResource, std::shared_ptr<RHIResource> pCounter, const RHIUnorderedAccessViewDesc& desc)
{
	check(viewIndex < _numViews);
	check(_type == ERHIShaderResourceViewType::CBV_SRV_UAV);
}