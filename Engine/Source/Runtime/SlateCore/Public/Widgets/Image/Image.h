// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/LeafWidget.h"
#include "Draw/SlateBrush.h"

interface IRHIBitmap;
interface IRHISolidColorBrush;

class SLATECORE_API SImage : public SLeafWidget
{
	GENERATED_BODY(SImage)

private:
	class SRenderElement;

private:
	SPROPERTY(ImageSource)
	IRHIBitmap* ImageSource = nullptr;
	SPROPERTY(TintBrush)
	IRHISolidColorBrush* TintBrush = nullptr;

	Vector2 ImageSize;
	Color TintColor;

public:
	SImage();

	virtual Vector2 GetDesiredSize() override;

	void SetBrush(const SlateBrush& InBrush);
	SlateBrush GetBrush();
	void SetTintColor(const Color& InTintColor);
	Color GetTintColor() const;

public:
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_ATTRIBUTE(SlateBrush, Brush)
		DECLARE_SLATE_ATTRIBUTE(Color, TintColor, NamedColors::White)
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();

protected:
	virtual int32 OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SSlateDrawCollector* DrawCollector, int32 InLayer, bool bParentEnabled) override;
};