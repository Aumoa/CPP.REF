// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/SCompoundWidget.h"
#include "Layout/ArrangedChildrens.h"

SCompoundWidget::SCompoundWidget() : Super()
{
}

void SCompoundWidget::Tick(const Geometry& AllottedGeometry, const TimeSpan& deltaTime)
{
    Super::Tick(AllottedGeometry, deltaTime);

    ArrangedChildrens ArrangedChildren(ESlateVisibility::All);
    ArrangeChildren(ArrangedChildren, AllottedGeometry);

    for (auto& Arranged : ArrangedChildren.GetWidgets())
    {
        Arranged.GetWidget()->Tick(Arranged.GetGeometry(), deltaTime);
    }
}

bool SCompoundWidget::PrepassLayout()
{
    bool bShouldBePrepass = false;
    size_t NumChildrens = this->NumChildrens();

    for (size_t ChildIndex = 0; ChildIndex < NumChildrens; ++ChildIndex)
    {
        if (GetChildrenAt(ChildIndex)->PrepassLayout())
        {
            bShouldBePrepass = true;
        }
    }

    if (ShouldBePrepassLayout() || bShouldBePrepass)
    {
        CacheDesiredSize();
        bShouldBePrepass = true;
    }

    return Super::PrepassLayout() || bShouldBePrepass;
}

bool SCompoundWidget::SendMouseMoved(const Geometry& AllottedGeometry, const MouseState& State)
{
    if (ESlateVisibility::AreChildrenHitTestVisible(GetVisibility()))
    {
        ArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
        ArrangeChildren(ArrangedChildren, AllottedGeometry);

        for (auto& Arranged : ArrangedChildren.GetWidgets())
        {
            if (Arranged.GetWidget()->SendMouseMoved(Arranged.GetGeometry(), State))
            {
                return true;
            }
        }
    }

    if (ESlateVisibility::IsHitTestVisible(GetVisibility()))
    {
        return OnReceiveMouseMoved(AllottedGeometry, State);
    }

    return false;
}

bool SCompoundWidget::SendMouseWheelScrolled(const Geometry& AllottedGeometry, int32 ScrollDelta, const MouseState& State)
{
    Vector2 CursorPos = State.GetCursorLocation();

    if (ESlateVisibility::AreChildrenHitTestVisible(GetVisibility()))
    {
        ArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
        ArrangeChildren(ArrangedChildren, AllottedGeometry);

        for (auto& Arranged : ArrangedChildren.GetWidgets())
        {
            if (Arranged.GetWidget()->SendMouseWheelScrolled(Arranged.GetGeometry(), ScrollDelta, State))
            {
                return true;
            }
        }
    }

    if (ESlateVisibility::IsHitTestVisible(GetVisibility()) &&
        AllottedGeometry.GetRenderBoundingRect().PtInRect(CursorPos))
    {
        return OnReceiveMouseWheelScrolled(AllottedGeometry, ScrollDelta, State);
    }

    return false;
}

bool SCompoundWidget::SendMouseEvent(const Geometry& AllottedGeometry, EMouseButton Button, EMouseButtonEvent Event, const MouseState& State)
{
    if (ESlateVisibility::AreChildrenHitTestVisible(GetVisibility()))
    {
        ArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
        ArrangeChildren(ArrangedChildren, AllottedGeometry);

        for (auto& Arranged : ArrangedChildren.GetWidgets())
        {
            if (Arranged.GetWidget()->SendMouseEvent(Arranged.GetGeometry(), Button, Event, State))
            {
                return true;
            }
        }
    }

    if (ESlateVisibility::IsHitTestVisible(GetVisibility()) &&
        AllottedGeometry.GetRenderBoundingRect().PtInRect(Vector<>::Cast<Vector2>(State.GetCursorLocation())))
    {
        return OnReceiveMouseEvent(AllottedGeometry, Button, Event, State);
    }

    return false;
}

bool SCompoundWidget::SendKeyboardEvent(const Geometry& AllottedGeometry, EKey Key, EKeyboardEvent Event, const KeyboardState& State)
{
    if (ESlateVisibility::AreChildrenHitTestVisible(GetVisibility()))
    {
        ArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
        ArrangeChildren(ArrangedChildren, AllottedGeometry);

        for (auto& Arranged : ArrangedChildren.GetWidgets())
        {
            if (Arranged.GetWidget()->SendKeyboardEvent(Arranged.GetGeometry(), Key, Event, State))
            {
                return true;
            }
        }
    }

    if (ESlateVisibility::IsHitTestVisible(GetVisibility()))
    {
        return OnReceiveKeyboardEvent(AllottedGeometry, Key, Event, State);
    }

    return false;
}

bool SCompoundWidget::SendIMEEvent(const Geometry& AllottedGeometry, const IMEEvent& EventArgs, const KeyboardState& State)
{
    if (ESlateVisibility::AreChildrenHitTestVisible(GetVisibility()))
    {
        ArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
        ArrangeChildren(ArrangedChildren, AllottedGeometry);

        for (auto& Arranged : ArrangedChildren.GetWidgets())
        {
            if (Arranged.GetWidget()->SendIMEEvent(Arranged.GetGeometry(), EventArgs, State))
            {
                return true;
            }
        }
    }

    if (ESlateVisibility::IsHitTestVisible(GetVisibility()))
    {
        return OnReceiveIMEEvent(AllottedGeometry, EventArgs, State);
    }

    return false;
}

int32 SCompoundWidget::OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SSlateDrawCollector* DrawCollector, int32 InLayer, bool bParentEnabled)
{
	ArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
	ArrangeChildren(ArrangedChildren, AllottedGeometry);

	return PaintArrangedChildrens(Args, ArrangedChildren, AllottedGeometry, CullingRect, DrawCollector, InLayer, bParentEnabled);
}

int32 SCompoundWidget::PaintArrangedChildrens(const PaintArgs& Args, ArrangedChildrens& ArrangedChildrens, const Geometry& AllottedGeometry, const Rect& CullingRect, SSlateDrawCollector* DrawCollector, int32 InLayer, bool bParentEnabled)
{
    bool bShouldBeEnabled = ShouldBeEnabled(bParentEnabled);

    for (const ArrangedWidget& Arranged : ArrangedChildrens.GetWidgets())
    {
        const std::shared_ptr<SWidget>& CurChild = Arranged.GetWidget();

        if (!IsChildWidgetCulled(CullingRect, Arranged))
        {
            int32 CurChildsMaxLayer = CurChild->Paint(Args, Arranged.GetGeometry(), CullingRect, DrawCollector, InLayer, bShouldBeEnabled);
            InLayer = MathEx::Max(CurChildsMaxLayer, InLayer);
        }
    }

    return InLayer;
}

bool SCompoundWidget::OnReceiveMouseMoved(const Geometry& AllottedGeometry, const MouseState& State)
{
    ArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
    ArrangeChildren(ArrangedChildren, AllottedGeometry);

    for (auto& Arranged : ArrangedChildren.GetWidgets())
    {
        if (Arranged.GetWidget()->SendMouseMoved(Arranged.GetGeometry(), State))
        {
            return true;
        }
    }

    return false;
}

bool SCompoundWidget::OnReceiveMouseWheelScrolled(const Geometry& AllottedGeometry, int32 ScrollDelta, const MouseState& State)
{
    ArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
    ArrangeChildren(ArrangedChildren, AllottedGeometry);

    for (auto& Arranged : ArrangedChildren.GetWidgets())
    {
        if (Arranged.GetWidget()->SendMouseWheelScrolled(Arranged.GetGeometry(), ScrollDelta, State))
        {
            return true;
        }
    }

    return false;
}

bool SCompoundWidget::OnReceiveMouseEvent(const Geometry& AllottedGeometry, EMouseButton Button, EMouseButtonEvent Event, const MouseState& State)
{
    ArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
    ArrangeChildren(ArrangedChildren, AllottedGeometry);

    for (auto& Arranged : ArrangedChildren.GetWidgets())
    {
        if (Arranged.GetWidget()->SendMouseEvent(Arranged.GetGeometry(), Button, Event, State))
        {
            return true;
        }
    }

    return false;
}

bool SCompoundWidget::OnReceiveKeyboardEvent(const Geometry& AllottedGeometry, EKey Key, EKeyboardEvent Event, const KeyboardState& State)
{
    ArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
    ArrangeChildren(ArrangedChildren, AllottedGeometry);

    for (auto& ArrangedWidget : ArrangedChildren.GetWidgets())
    {
        if (ArrangedWidget.GetWidget()->SendKeyboardEvent(ArrangedWidget.GetGeometry(), Key, Event, State))
        {
            return true;
        }
    }

    return false;
}

bool SCompoundWidget::OnReceiveIMEEvent(const Geometry& AllottedGeometry, const IMEEvent& EventArgs, const KeyboardState& State)
{
    ArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
    ArrangeChildren(ArrangedChildren, AllottedGeometry);

    for (auto& ArrangedWidget : ArrangedChildren.GetWidgets())
    {
        if (ArrangedWidget.GetWidget()->SendIMEEvent(ArrangedWidget.GetGeometry(), EventArgs, State))
        {
            return true;
        }
    }

    return false;
}