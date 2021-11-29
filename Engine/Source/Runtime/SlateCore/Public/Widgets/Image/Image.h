// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/LeafWidget.h"
#include "Draw/SlateBrush.h"

class STexture2D;

class SLATECORE_API SImage : public SLeafWidget
{
	GENERATED_BODY(SImage)

private:
	SlateBrush Brush;

public:
	SImage();
	virtual ~SImage() override;

	virtual Vector2 GetDesiredSize() override;

	void SetBrush(const SlateBrush& InBrush);
	SlateBrush GetBrush();

public:
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_ATTRIBUTE(SlateBrush, Brush)
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();

protected:
	virtual int32 OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SlateWindowElementList& InDrawElements, int32 InLayer, bool bParentEnabled) override;
};