// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/Image/SImage.h"
//#include "Draw/SlateDrawCollector.h"

SImage::SImage() : Super()
{
	SetRenderTransform(SlateRenderTransform::Identity());
}

//void SImage::SetBrush(const SlateBrush& InBrush)
//{
//	//if (ImageSource != InBrush.ImageSource)
//	//{
//	//	ImageSource = InBrush.ImageSource;
//	//	CachedRenderElement = nullptr;
//	//}
//
//	//if (ImageSize != InBrush.ImageSize)
//	//{
//	//	ImageSize = InBrush.ImageSize;
//	//	InvalidateLayoutAndVolatility();
//	//}
//}

//SlateBrush SImage::GetBrush()
//{
//	//return SlateBrush(ImageSource, ImageSize);
//	throw;
//}

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
	//SetBrush(Attr._Brush);
	SetTintColor(Attr._TintColor);
}

Vector2 SImage::ComputeDesiredSize()
{
	return ImageSize;
}

int32 SImage::OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SSlateDrawCollector* DrawCollector, int32 InLayer, bool bParentEnabled)
{
	//if (ImageSource == nullptr)
	//{
	//	if (TintBrush)
	//	{
	//		TintBrush->SetColor(TintColor);
	//	}
	//	else
	//	{
	//		TintBrush = Args.Device->CreateSolidColorBrush(TintColor);
	//	}
	//}

	//if (CachedRenderElement == nullptr)
	//{
	//	CachedRenderElement = gcnew SRenderElement(this,
	//		Args,
	//		InLayer,
	//		AllottedGeometry
	//	);
	//}
	//else
	//{
	//	//CachedRenderElement->SetGeometry_GameThread(InLayer, AllottedGeometry);
	//}

	//if (CachedRenderElement)
	//{
	//	//DrawCollector->AddRenderElement(CachedRenderElement);
	//}
	return InLayer;
}