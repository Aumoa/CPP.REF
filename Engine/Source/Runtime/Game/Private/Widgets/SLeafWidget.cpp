// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/SLeafWidget.h"

using namespace ::libty;

SLeafWidget::SLeafWidget() : Super()
{
}

SLeafWidget::~SLeafWidget() noexcept
{
}

bool SLeafWidget::PrepassLayout()
{
    bool bShouldBePrepass = false;
    if (ShouldBePrepassLayout())
    {
        CacheDesiredSize();
        bShouldBePrepass = true;
    }

    return Super::PrepassLayout() || bShouldBePrepass;
}

bool SLeafWidget::SendMouseMoved(const Geometry& AllottedGeometry, const MouseState& State)
{
    if (ESlateVisibility::IsHitTestVisible(GetVisibility()))
    {
        return OnReceiveMouseMoved(AllottedGeometry, State);
    }

    return false;
}

bool SLeafWidget::SendMouseWheelScrolled(const Geometry& AllottedGeometry, int32 ScrollDelta, const MouseState& State)
{
    Vector2 CursorPos = State.CursorLocation;

    if (ESlateVisibility::IsHitTestVisible(GetVisibility()) && AllottedGeometry.GetRenderBoundingRect().PtInRect(CursorPos))
    {
        return OnReceiveMouseWheelScrolled(AllottedGeometry, ScrollDelta, State);
    }

    return false;
}

bool SLeafWidget::SendMouseEvent(const Geometry& AllottedGeometry, EMouseButton Button, EMouseButtonEvent Event, const MouseState& State)
{
    if (ESlateVisibility::IsHitTestVisible(GetVisibility()) &&
        AllottedGeometry.GetRenderBoundingRect().PtInRect(Vector<>::Cast<float>(State.CursorLocation)))
    {
        return OnReceiveMouseEvent(AllottedGeometry, Button, Event, State);
    }

    return false;
}

bool SLeafWidget::SendKeyboardEvent(const Geometry& AllottedGeometry, EKey Key, EKeyboardEvent Event, const KeyboardState& State)
{
    if (ESlateVisibility::IsHitTestVisible(GetVisibility()))
    {
        return OnReceiveKeyboardEvent(AllottedGeometry, Key, Event, State);
    }

    return false;
}

bool SLeafWidget::SendIMEEvent(const Geometry& AllottedGeometry, const IMEEvent& EventArgs, const KeyboardState& State)
{
    if (ESlateVisibility::IsHitTestVisible(GetVisibility()))
    {
        return OnReceiveIMEEvent(AllottedGeometry, EventArgs, State);
    }

    return false;
}

void SLeafWidget::OnArrangeChildren(ArrangedChildrens& ArrangedChildrens, const Geometry& AllottedGeometry)
{
}

bool SLeafWidget::OnReceiveMouseMoved(const Geometry& AllottedGeometry, const MouseState& State)
{
    return false;
}

bool SLeafWidget::OnReceiveMouseWheelScrolled(const Geometry& AllottedGeometry, int32 ScrollDelta, const MouseState& State)
{
    return false;
}

bool SLeafWidget::OnReceiveMouseEvent(const Geometry& AllottedGeometry, EMouseButton Button, EMouseButtonEvent Event, const MouseState& State)
{
	return false;
}

bool SLeafWidget::OnReceiveKeyboardEvent(const Geometry& AllottedGeometry, EKey Key, EKeyboardEvent Event, const KeyboardState& State)
{
    return false;
}

bool SLeafWidget::OnReceiveIMEEvent(const Geometry& AllottedGeometry, const IMEEvent& EventArgs, const KeyboardState& State)
{
    return false;
}