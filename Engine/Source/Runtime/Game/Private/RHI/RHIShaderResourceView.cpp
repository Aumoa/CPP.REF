// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/RHIShaderResourceView.h"

RHIShaderResourceView::RHIShaderResourceView(std::shared_ptr<RHIDevice> device)
	: RHIDeviceResource(std::move(device))
{
}

RHIShaderResourceView::~RHIShaderResourceView() noexcept
{
}