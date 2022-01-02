// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/Image/Image.h"
#include "Draw/PaintArgs.h"
#include "Draw/IRenderSlateElement.h"
#include "Draw/SlateDrawCollector.h"
#include "RHI/IRHIDeviceContext2D.h"
#include "RHI/IRHISolidColorBrush.h"
#include "RHI/IRHIDevice.h"
#include "RHI/IRHIBitmap.h"

class SImage::SRenderElement : implements SObject, implements IRenderSlateElement
{
	GENERATED_BODY(SRenderElement)

public:
	const PaintArgs Args;

	const int32 Layer = 0;
	const Geometry CachedGeometry;

	const Vector2 ImageSize;
	SPROPERTY(ImageSource)
	IRHIBitmap* ImageSource = nullptr;
	SPROPERTY(TintBrush)
	IRHISolidColorBrush* TintBrush = nullptr;

public:
	SRenderElement(SImage* Source, const PaintArgs& InPaintArgs, int32 InLayer, const Geometry& AllottedGeometry)
		: Super()
		, Args(InPaintArgs)
		, Layer(InLayer)
		, CachedGeometry(AllottedGeometry)

		, ImageSize(Source->ImageSize)
		, ImageSource(Source->ImageSource)
		, TintBrush(Source->TintBrush)
	{
	}

	virtual int32 GetLayer() override
	{
		return Layer;
	}

	virtual Geometry GetCachedGeometry() override
	{
		return CachedGeometry;
	}

	virtual void RenderElement(IRHIDeviceContext2D* CommandBuffer, const LocalRenderLayout& LocalLayout) override
	{
		Vector2 RB = LocalLayout.LocalPosition + LocalLayout.LocalSize;

		Rect DrawRect;
		DrawRect.Left = LocalLayout.LocalPosition.X;
		DrawRect.Top = LocalLayout.LocalPosition.Y;
		DrawRect.Right = RB.X;
		DrawRect.Bottom = RB.Y;

		if (ImageSource)
		{
			CommandBuffer->DrawBitmap(ImageSource, &DrawRect, Args.RenderOpacity, ERHIInterpolationMode::Anisotropic);
		}
		else
		{
			CommandBuffer->FillRectangle(TintBrush, DrawRect);
		}
	}
};

GENERATE_BODY(SImage);
GENERATE_BODY(SImage::SRenderElement);

SImage::SImage() : Super()
{
	SetRenderTransform(SlateRenderTransform::Identity());
}

Vector2 SImage::GetDesiredSize()
{
	return ImageSize;
}

void SImage::SetBrush(const SlateBrush& InBrush)
{
	ImageSource = InBrush.ImageSource;
	ImageSize = InBrush.ImageSize;
}

SlateBrush SImage::GetBrush()
{
	return SlateBrush(ImageSource, ImageSize);
}

void SImage::SetTintColor(const Color& InTintColor)
{
	TintColor = InTintColor;
}

Color SImage::GetTintColor() const
{
	return TintColor;
}

DEFINE_SLATE_CONSTRUCTOR(SImage, Attr)
{
	INVOKE_SLATE_CONSTRUCTOR_SUPER(Attr);
	SetBrush(Attr._Brush);
	SetTintColor(Attr._TintColor);
}

int32 SImage::OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SSlateDrawCollector* DrawCollector, int32 InLayer, bool bParentEnabled)
{
	if (ImageSource == nullptr)
	{
		if (TintBrush)
		{
			TintBrush->SetColor(TintColor);
		}
		else
		{
			TintBrush = Args.Device->CreateSolidColorBrush(TintColor);
		}
	}

	DrawCollector->AddRenderElement(gcnew SRenderElement(this,
		Args.WithNewParent(this),
		InLayer,
		AllottedGeometry
	));

	return InLayer;
}