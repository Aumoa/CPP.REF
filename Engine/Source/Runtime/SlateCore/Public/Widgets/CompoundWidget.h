// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widget.h"

class SLATECORE_API SCompoundWidget : public SWidget
{
	GENERATED_BODY(SCompoundWidget)

public:
	SCompoundWidget();

	virtual void Tick(const Geometry& AllottedGeometry, float InDeltaTime);

protected:
	virtual int32 OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SlateWindowElementList& InDrawElements, int32 InLayer, bool bParentEnabled) override;

	int32 PaintArrangedChildrens(const PaintArgs& Args, ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry, const Rect& CullingRect, SlateWindowElementList& InDrawElements, int32 InLayer, bool bParentEnabled);

	virtual bool OnReceiveMouseMoved(const Geometry& AllottedGeometry, const Vector2N& Location) override;
	virtual bool OnReceiveMouseWheelScrolled(const Geometry& AllottedGeometry, int32 ScrollDelta) override;
	virtual bool OnReceiveMouseEvent(const Geometry& AllottedGeometry, const Vector2N& Location, EMouseButton Button, EMouseButtonEvent Event) override;
	virtual bool OnReceiveKeyboardEvent(const Geometry& AllottedGeometry, EKey Key, EKeyboardEvent Event) override;
};