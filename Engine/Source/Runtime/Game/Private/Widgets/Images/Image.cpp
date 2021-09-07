// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Widgets/Images/Image.h"
#include "Assets/Texture2D.h"

SImage::SImage(const std::wstring& name) : Super(name)
{
	SetRenderTransform(SlateRenderTransform::Identity());
}

SImage::~SImage()
{
}

Vector2 SImage::GetDesiredSize() const
{
	return _brush.ImageSize;
}

void SImage::SetBrush(const SlateBrush& brush)
{
	_brush = brush;
}

void SImage::SetTexture2D(Texture2D* texture, bool bMatchSize)
{
	_brush.ImageSource = texture->GetShaderResourceView();
	
	if (bMatchSize)
	{
		int32 width, height;
		texture->GetPixelSize(&width, &height);
		_brush.ImageSize = Vector2((float)width, (float)height);
	}
}

SlateBrush SImage::GetBrush() const
{
	return _brush;
}

DEFINE_SLATE_CONSTRUCTOR(SImage, attr)
{
	Super::Construct(attr);
	SetBrush(attr._Brush);
}

int32 SImage::OnPaint(PaintArgs* paintArgs, const Geometry& allottedGeometry, const Rect& cullingRect, SlateWindowElementList* drawElements, int32 layer, bool bParentEnabled) const
{
	SlateDrawElement element;
	element.Brush = _brush;
	element.Layer = layer;
	element.Transform = allottedGeometry.ToPaintGeometry();

	drawElements->Add(element);
	return layer;
}

std::wstring SImage::GenerateAutoNumberedName()
{
	static std::atomic<size_t> number;
	static std::wstring wc_name = ANSI_TO_WCHAR(typeid(SImage).name());
	return std::format(L"{}_{}", wc_name, number++);
}