// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widget.h"

class GAME_API SCompoundWidget : public SWidget
{
	GENERATED_BODY(SCompoundWidget)

public:
	SCompoundWidget();

protected:
	virtual int32 OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SlateWindowElementList& InDrawElements, int32 InLayer, bool bParentEnabled) override;

	int32 PaintArrangedChildrens(const PaintArgs& Args, ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry, const Rect& CullingRect, SlateWindowElementList& InDrawElements, int32 InLayer, bool bParentEnabled);

	virtual bool OnReceiveMouseEvent(const Geometry& allottedGeometry, const Vector2N& location, EMouseButton button, EMouseButtonEvent event) override;
	virtual bool OnReceiveKeyboardEvent(const Geometry& allottedGeometry, EKey key, EKeyboardEvent event) override;
};