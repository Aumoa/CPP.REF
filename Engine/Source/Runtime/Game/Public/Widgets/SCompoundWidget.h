// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SWidget.h"

class GAME_API SCompoundWidget : public SWidget
{
private:
	using Super = SWidget;
	using This = SCompoundWidget;

public:
	SCompoundWidget();

	virtual void Tick(const Geometry& AllottedGeometry, const TimeSpan& deltaTime);
	virtual bool PrepassLayout();

	virtual bool SendMouseMoved(const Geometry& AllottedGeometry, const MouseState& State) override;
	virtual bool SendMouseWheelScrolled(const Geometry& AllottedGeometry, int32 ScrollDelta, const MouseState& State) override;
	virtual bool SendMouseEvent(const Geometry& AllottedGeometry, EMouseButton Button, EMouseButtonEvent Event, const MouseState& State) override;
	virtual bool SendKeyboardEvent(const Geometry& AllottedGeometry, EKey Key, EKeyboardEvent Event, const KeyboardState& State) override;
	virtual bool SendIMEEvent(const Geometry& AllottedGeometry, const IMEEvent& EventArgs, const KeyboardState& State) override;

	virtual size_t NumChildrens() = 0;
	virtual SWidget* GetChildrenAt(size_t IndexOf) = 0;

protected:
	virtual int32 OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SSlateDrawCollector* DrawCollector, int32 InLayer, bool bParentEnabled) override;

	int32 PaintArrangedChildrens(const PaintArgs& Args, ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry, const Rect& CullingRect, SSlateDrawCollector* DrawCollector, int32 InLayer, bool bParentEnabled);

	virtual bool OnReceiveMouseMoved(const Geometry& AllottedGeometry, const MouseState& State);
	virtual bool OnReceiveMouseWheelScrolled(const Geometry& AllottedGeometry, int32 ScrollDelta, const MouseState& State);
	virtual bool OnReceiveMouseEvent(const Geometry& AllottedGeometry, EMouseButton Button, EMouseButtonEvent Event, const MouseState& State);
	virtual bool OnReceiveKeyboardEvent(const Geometry& AllottedGeometry, EKey Key, EKeyboardEvent Event, const KeyboardState& State);
	virtual bool OnReceiveIMEEvent(const Geometry& AllottedGeometry, const IMEEvent& EventArgs, const KeyboardState& State);
};