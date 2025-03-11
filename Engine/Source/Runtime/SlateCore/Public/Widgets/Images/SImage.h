// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SLeafWidget.h"
#include "Rendering/SlateBrush.h"

class SLATECORE_API SImage : public SLeafWidget
{
	GENERATED_SLATE_BODY(SImage)

private:
	NSlateBrush Brush;

public:
	SImage();

protected:
	virtual Vector2 ComputeDesiredSize() const override;
	virtual int32 OnPaint(const NPaintArgs& Args, const NGeometry& AllottedGeometry, const Rect& CullingRect, NSlateWindowElementList& OutDrawElements, int32 InLayer, bool bParentEnabled) const override;

public:
	BEGIN_SLATE_ATTRIBUTE()
		DECLARE_SLATE_ATTRIBUTE(NSlateBrush, Brush)
	END_SLATE_ATTRIBUTE();

	DECLARE_SLATE_CONSTRUCTOR();
};