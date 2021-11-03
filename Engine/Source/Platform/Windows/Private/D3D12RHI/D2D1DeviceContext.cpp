// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D2D1DeviceContext.h"
#include "D2D1Brush.h"
#include "D3D12Texture2D.h"

SD2D1DeviceContext::SD2D1DeviceContext(SDXGIFactory* InFactory, SD3D12Device* InDevice, ID2D1DeviceContext* InDeviceContext, ID3D11On12Device* InteropDevice) : Super(InFactory, InDevice, nullptr)
	, InteropDev(InteropDevice)
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
	SetTarget(nullptr);
}

void SD2D1DeviceContext::SetTarget(IRHITexture2D* InTarget)
{
	auto Target_s = Cast<SD3D12Texture2D>(InTarget);
	if (Target_s)
	{
		DeviceContext->SetTarget(Target_s->Get<ID2D1Bitmap1>());
		if (Target_s != WeakTarget.lock().get())
		{
			ReleaseWrappedTarget();
			WeakTarget = InTarget->weak_from_this();
			AcquireWrappedTarget();
		}
	}
	else
	{
		ReleaseWrappedTarget();
		DeviceContext->SetTarget(nullptr);
	}
}

void SD2D1DeviceContext::FillRectangle(IRHIBrush* InBrush, const Rect& Region)
{
	auto Brush_s = Cast<SD2D1Brush>(InBrush);
	DeviceContext->FillRectangle((const D2D1_RECT_F&)Region, Brush_s->Get<ID2D1Brush>());
}

void SD2D1DeviceContext::AcquireWrappedTarget()
{
	if (auto Ptr = WeakTarget.lock(); Ptr)
	{
		auto CastPtr = std::dynamic_pointer_cast<SD3D12Texture2D>(Ptr);
		auto Resource = CastPtr->Get<ID3D11Resource>();
		InteropDev->AcquireWrappedResources(&Resource, 1);
	}
}

void SD2D1DeviceContext::ReleaseWrappedTarget()
{
	if (auto Ptr = WeakTarget.lock(); Ptr)
	{
		auto CastPtr = std::dynamic_pointer_cast<SD3D12Texture2D>(Ptr);
		auto Resource = CastPtr->Get<ID3D11Resource>();
		InteropDev->ReleaseWrappedResources(&Resource, 1);

		WeakTarget.reset();
	}
}