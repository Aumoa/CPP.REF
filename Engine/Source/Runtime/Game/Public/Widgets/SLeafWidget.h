// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "SWidget.h"

class GAME_API SLeafWidget : public SWidget
{
	using This = SLeafWidget;
	using Super = SWidget;

public:
	SLeafWidget();
	virtual ~SLeafWidget() noexcept override;

	virtual bool PrepassLayout() override;

	virtual bool SendMouseMoved(const Geometry& AllottedGeometry, const MouseState& State) override;
	virtual bool SendMouseWheelScrolled(const Geometry& AllottedGeometry, int32 ScrollDelta, const MouseState& State) override;
	virtual bool SendMouseEvent(const Geometry& AllottedGeometry, EMouseButton Button, EMouseButtonEvent Event, const MouseState& State) override;
	virtual bool SendKeyboardEvent(const Geometry& AllottedGeometry, EKey Key, EKeyboardEvent Event, const KeyboardState& State) override;
	virtual bool SendIMEEvent(const Geometry& AllottedGeometry, const IMEEvent& EventArgs, const KeyboardState& State) override;

protected:
	virtual void OnArrangeChildren(ArrangedChildrens& ArrangedChildrens, const Geometry& AllottedGeometry) override;

	virtual bool OnReceiveMouseMoved(const Geometry& AllottedGeometry, const MouseState& State);
	virtual bool OnReceiveMouseWheelScrolled(const Geometry& AllottedGeometry, int32 ScrollDelta, const MouseState& State);
	virtual bool OnReceiveMouseEvent(const Geometry& AllottedGeometry, EMouseButton Button, EMouseButtonEvent Event, const MouseState& State);
	virtual bool OnReceiveKeyboardEvent(const Geometry& AllottedGeometry, EKey Key, EKeyboardEvent Event, const KeyboardState& State);
	virtual bool OnReceiveIMEEvent(const Geometry& AllottedGeometry, const IMEEvent& EventArgs, const KeyboardState& State);
};