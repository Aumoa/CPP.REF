// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D2D1DeviceContext.h"
#include "D2D1Brush.h"
#include "D2D1Bitmap.h"
#include "DWriteTextLayout.h"

GENERATE_BODY(SD2D1DeviceContext);

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

void SD2D1DeviceContext::SetTarget(IRHIImage* InTarget, std::optional<Color> ClearColor)
{
	auto Target_s = Cast<SD2D1Bitmap>(InTarget);
	if (Target_s)
	{
		DeviceContext->SetTarget(Target_s->Get<ID2D1Bitmap1>());

		if (ClearColor.has_value())
		{
			DeviceContext->Clear((const D2D1_COLOR_F&)*ClearColor);
		}

		if (InTarget != Target)
		{
			ReleaseWrappedTarget();
			Target = InTarget;
			AcquireWrappedTarget();
		}

		const bool bMultisampled = Target_s->IsMultisampled();
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

void SD2D1DeviceContext::FillEllipse(IRHIBrush* InBrush, const Rect& Region)
{
	auto Brush_s = Cast<SD2D1Brush>(InBrush);
	D2D1_ELLIPSE Ellipse;
	Ellipse.point = (const D2D1_POINT_2F&)Region.GetCenter();
	Ellipse.radiusX = Region.GetWidth() * 0.5f;
	Ellipse.radiusY = Region.GetHeight() * 0.5f;
	DeviceContext->FillEllipse(Ellipse, Brush_s->Get<ID2D1Brush>());
}

void SD2D1DeviceContext::DrawTextLayout(const Vector2& DrawOrigin, IRHITextLayout* Layout, IRHIBrush* InBrush, ERHIDrawTextOptions Options)
{
	auto Layout_s = Cast<SDWriteTextLayout>(Layout);
	auto Brush_s = Cast<SD2D1Brush>(InBrush);
	DeviceContext->DrawTextLayout((const D2D1_POINT_2F&)DrawOrigin, Layout_s->Get<IDWriteTextLayout>(), Brush_s->Get<ID2D1Brush>(), (D2D1_DRAW_TEXT_OPTIONS)Options);
}

void SD2D1DeviceContext::DrawImage(IRHIImage* Image, const Vector2* Offset, const Rect* SourceRect, ERHIInterpolationMode InterpolationMode, ERHICompositeMode CompositeMode)
{
	auto Image_s = Cast<SD2D1Image>(Image);
	auto Image_r = Image_s->Get<ID2D1Image>();
	DeviceContext->DrawImage(Image_r, (const D2D1_POINT_2F*)Offset, (const D2D1_RECT_F*)SourceRect, (D2D1_INTERPOLATION_MODE)InterpolationMode, (D2D1_COMPOSITE_MODE)CompositeMode);
}

void SD2D1DeviceContext::DrawBitmap(IRHIBitmap* Bitmap, const Rect* DestRect, float Opacity, ERHIInterpolationMode InterpolationMode, const Rect* SourceRect, const Matrix4x4* PerspectiveTransform)
{
	auto Bitmap_s = Cast<SD2D1Bitmap>(Bitmap);
	auto Bitmap_r = Bitmap_s->Get<ID2D1Bitmap1>();
	DeviceContext->DrawBitmap(Bitmap_r, (const D2D1_RECT_F*)DestRect, Opacity, (D2D1_INTERPOLATION_MODE)InterpolationMode, (const D2D1_RECT_F*)SourceRect, (const D2D1_MATRIX_4X4_F*)PerspectiveTransform);
}

void SD2D1DeviceContext::AcquireWrappedTarget()
{
	if (auto CastPtr = Cast<SD2D1Bitmap>(Target))
	{
		auto Resource = CastPtr->Get<ID3D11Resource>();
		if (Resource)
		{
			InteropDev->AcquireWrappedResources(&Resource, 1);
		}
	}
}

void SD2D1DeviceContext::ReleaseWrappedTarget()
{
	if (auto CastPtr = Cast<SD2D1Bitmap>(Target))
	{
		auto Resource = CastPtr->Get<ID3D11Resource>();
		if (Resource)
		{
			InteropDev->ReleaseWrappedResources(&Resource, 1);
		}
	}
	Target = nullptr;
}