// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widget.h"

class GAME_API SLeafWidget : public SWidget
{
	GENERATED_BODY(SLeafWidget)

public:
	SLeafWidget();
	virtual ~SLeafWidget() override;

	virtual bool PrepassLayout() override;

	virtual bool SendMouseMoved(const Geometry& AllottedGeometry, const Vector2N& Location) override;
	virtual bool SendMouseWheelScrolled(const Geometry& AllottedGeometry, int32 ScrollDelta) override;
	virtual bool SendMouseEvent(const Geometry& AllottedGeometry, const Vector2N& Location, EMouseButton Button, EMouseButtonEvent Event) override;
	virtual bool SendKeyboardEvent(const Geometry& AllottedGeometry, EKey Key, EKeyboardEvent Event) override;
	virtual bool SendIMEEvent(const Geometry& AllottedGeometry, const IMEEvent& EventArgs) override;

protected:
	virtual void OnArrangeChildren(ArrangedChildrens& ArrangedChildrens, const Geometry& AllottedGeometry) override;

	virtual bool OnReceiveMouseMoved(const Geometry& AllottedGeometry, const Vector2N& Location) override;
	virtual bool OnReceiveMouseWheelScrolled(const Geometry& AllottedGeometry, int32 ScrollDelta) override;
	virtual bool OnReceiveMouseEvent(const Geometry& AllottedGeometry, const Vector2N& Location, EMouseButton Button, EMouseButtonEvent Event) override;
	virtual bool OnReceiveKeyboardEvent(const Geometry& AllottedGeometry, EKey Key, EKeyboardEvent Event) override;
	virtual bool OnReceiveIMEEvent(const Geometry& AllottedGeometry, const IMEEvent& EventArgs) override;
};