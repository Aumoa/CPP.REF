// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D2D1DeviceContext.h"

SD2D1DeviceContext::SD2D1DeviceContext(SDXGIFactory* InFactory, SD3D12Device* InDevice, ID2D1DeviceContext* InDeviceContext) : Super(InFactory, InDevice, nullptr)
	, DeviceContext(InDeviceContext)
{
}

void SD2D1DeviceContext::BeginDraw()
{
	DeviceContext->BeginDraw();
}

void SD2D1DeviceContext::EndDraw()
{
	HR(DeviceContext->EndDraw());
}