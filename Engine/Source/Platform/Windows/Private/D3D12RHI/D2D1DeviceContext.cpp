// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D2D1DeviceContext.h"
#include "D2D1Brush.h"
#include "D3D12Texture2D.h"
#include "DWriteTextLayout.h"

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
	SetTarget(nullptr, std::nullopt);
}

void SD2D1DeviceContext::SetTarget(IRHITexture2D* InTarget, std::optional<Color> ClearColor)
{
	auto Target_s = Cast<SD3D12Texture2D>(InTarget);
	if (Target_s)
	{
		DeviceContext->SetTarget(Target_s->Get<ID2D1Bitmap1>());

		if (ClearColor.has_value())
		{
			DeviceContext->Clear((const D2D1_COLOR_F&)*ClearColor);
		}

		if (Target_s != WeakTarget.lock().get())
		{
			ReleaseWrappedTarget();
			WeakTarget = InTarget->weak_from_this();
			AcquireWrappedTarget();
		}

		const bool bMultisampled = InTarget->GetDesc().SampleDesc.Count != 1;
		DeviceContext->SetAntialiasMode(bMultisampled ? D2D1_ANTIALIAS_MODE_ALIASED : D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	}
	else
	{
		ReleaseWrappedTarget();
		DeviceContext->SetTarget(nullptr);
	}
}

void SD2D1DeviceContext::SetTransform(const Matrix3x2& Transform)
{
	DeviceContext->SetTransform((const D2D1_MATRIX_3X2_F&)Transform);
}

Matrix3x2 SD2D1DeviceContext::GetTransform()
{
	D2D1_MATRIX_3X2_F M;
	DeviceContext->GetTransform(&M);
	return (const Matrix3x2&)M;
}

void SD2D1DeviceContext::FillRectangle(IRHIBrush* InBrush, const Rect& Region)
{
	auto Brush_s = Cast<SD2D1Brush>(InBrush);
	DeviceContext->FillRectangle((const D2D1_RECT_F&)Region, Brush_s->Get<ID2D1Brush>());
}

void SD2D1DeviceContext::DrawTextLayout(const Vector2& DrawOrigin, IRHITextLayout* Layout, IRHIBrush* InBrush, ERHIDrawTextOptions Options)
{
	auto Layout_s = Cast<SDWriteTextLayout>(Layout);
	auto Brush_s = Cast<SD2D1Brush>(InBrush);
	DeviceContext->DrawTextLayout((const D2D1_POINT_2F&)DrawOrigin, Layout_s->Get<IDWriteTextLayout>(), Brush_s->Get<ID2D1Brush>(), (D2D1_DRAW_TEXT_OPTIONS)Options);
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