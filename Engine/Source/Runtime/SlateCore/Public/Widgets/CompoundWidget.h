// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "Widget.h"

class SLATECORE_API SCompoundWidget : public SWidget
{
	CLASS_BODY(SCompoundWidget)

public:
	SCompoundWidget();

protected:
	virtual int32 OnPaint(PaintArgs* paintArgs, const Geometry& allottedGeometry, const Rect& cullingRect, SlateWindowElementList* drawElements, int32 layer, bool bParentEnabled) const override;

	int32 PaintArrangedChildrens(PaintArgs* paintArgs, ArrangedChildrens* arrangedChildrens, const Geometry& allottedGeometry, const Rect& cullingRect, SlateWindowElementList* drawElements, int32 layer, bool bParentEnabled) const;
};