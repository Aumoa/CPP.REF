// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/LeafWidget.h"
#include "Layout/LayoutImpl.h"
#include "IApplicationInterface.h"
#include "Input/IPlatformMouse.h"

GENERATE_BODY(SLeafWidget);

SLeafWidget::SLeafWidget() : Super()
{
}

SLeafWidget::~SLeafWidget()
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

bool SLeafWidget::SendMouseMoved(const Geometry& AllottedGeometry, const Vector2N& Location)
{
    Super::SendMouseMoved(AllottedGeometry, Location);

    if (SlateVisibilityExtensions::IsHitTestVisible(GetVisibility()))
    {
        return OnReceiveMouseMoved(AllottedGeometry, Location);
    }

    return false;
}

bool SLeafWidget::SendMouseWheelScrolled(const Geometry& AllottedGeometry, int32 ScrollDelta)
{
    Super::SendMouseWheelScrolled(AllottedGeometry, ScrollDelta);

    auto& PlatformMouse = IApplicationInterface::Get().GetPlatformMouse();
    auto State = PlatformMouse.GetState();
    Vector2 CursorPos = Vector2((float)State.X, (float)State.Y);

    if (SlateVisibilityExtensions::IsHitTestVisible(GetVisibility()) &&
        AllottedGeometry.GetRenderBoundingRect().PtInRect(CursorPos))
    {
        return OnReceiveMouseWheelScrolled(AllottedGeometry, ScrollDelta);
    }

    return false;
}

bool SLeafWidget::SendMouseEvent(const Geometry& AllottedGeometry, const Vector2N& Location, EMouseButton Button, EMouseButtonEvent Event)
{
    Super::SendMouseEvent(AllottedGeometry, Location, Button, Event);

    if (SlateVisibilityExtensions::IsHitTestVisible(GetVisibility()) &&
        AllottedGeometry.GetRenderBoundingRect().PtInRect(Location.Cast<float>()))
    {
        return OnReceiveMouseEvent(AllottedGeometry, Location, Button, Event);
    }

    return false;
}

bool SLeafWidget::SendKeyboardEvent(const Geometry& AllottedGeometry, EKey Key, EKeyboardEvent Event)
{
    Super::SendKeyboardEvent(AllottedGeometry, Key, Event);

    if (SlateVisibilityExtensions::IsHitTestVisible(GetVisibility()))
    {
        return OnReceiveKeyboardEvent(AllottedGeometry, Key, Event);
    }

    return false;
}

bool SLeafWidget::SendIMEEvent(const Geometry& AllottedGeometry, const IMEEvent& EventArgs)
{
    Super::SendIMEEvent(AllottedGeometry, EventArgs);

    if (SlateVisibilityExtensions::IsHitTestVisible(GetVisibility()))
    {
        return OnReceiveIMEEvent(AllottedGeometry, EventArgs);
    }

    return false;
}

void SLeafWidget::OnArrangeChildren(ArrangedChildrens& ArrangedChildrens, const Geometry& AllottedGeometry)
{
}

bool SLeafWidget::OnReceiveMouseMoved(const Geometry& AllottedGeometry, const Vector2N& Location)
{
    return false;
}

bool SLeafWidget::OnReceiveMouseWheelScrolled(const Geometry& AllottedGeometry, int32 ScrollDelta)
{
    return false;
}

bool SLeafWidget::OnReceiveMouseEvent(const Geometry& AllottedGeometry, const Vector2N& Location, EMouseButton Button, EMouseButtonEvent Event)
{
	return false;
}

bool SLeafWidget::OnReceiveKeyboardEvent(const Geometry& AllottedGeometry, EKey Key, EKeyboardEvent Event)
{
    return false;
}

bool SLeafWidget::OnReceiveIMEEvent(const Geometry& AllottedGeometry, const IMEEvent& EventArgs)
{
    return false;
}