// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/SCompoundWidget.h"
#include "Draw/PaintArgs.h"
#include "Layout/ArrangedChildrens.h"

using namespace ::libty;

SCompoundWidget::SCompoundWidget() : Super()
{
}

void SCompoundWidget::Tick(const Geometry& AllottedGeometry, float InDeltaTime)
{
    Super::Tick(AllottedGeometry, InDeltaTime);

    ArrangedChildrens ArrangedChildren(ESlateVisibility::All);
    ArrangeChildren(ArrangedChildren, AllottedGeometry);

    for (auto& Arranged : ArrangedChildren.GetWidgets())
    {
        Arranged.GetWidget()->Tick(Arranged.GetGeometry(), InDeltaTime);
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

bool SCompoundWidget::SendMouseMoved(const Geometry& AllottedGeometry, const Vector2N& Location)
{
    Super::SendMouseMoved(AllottedGeometry, Location);

    if (GetVisibility().AreChildrenHitTestVisible())
    {
        ArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
        ArrangeChildren(ArrangedChildren, AllottedGeometry);

        for (auto& Arranged : ArrangedChildren.GetWidgets())
        {
            if (Arranged.GetWidget()->SendMouseMoved(Arranged.GetGeometry(), Location))
            {
                return true;
            }
        }
    }

    if (GetVisibility().IsHitTestVisible())
    {
        return OnReceiveMouseMoved(AllottedGeometry, Location);
    }

    return false;
}

bool SCompoundWidget::SendMouseWheelScrolled(const Geometry& AllottedGeometry, int32 ScrollDelta)
{
    Super::SendMouseWheelScrolled(AllottedGeometry, ScrollDelta);

    auto& PlatformMouse = IApplicationInterface::Get().GetPlatformMouse();
    auto State = PlatformMouse.GetState();
    Vector2 CursorPos = Vector2((float)State.X, (float)State.Y);

    if (GetVisibility().AreChildrenHitTestVisible())
    {
        ArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
        ArrangeChildren(ArrangedChildren, AllottedGeometry);

        for (auto& Arranged : ArrangedChildren.GetWidgets())
        {
            if (Arranged.GetWidget()->SendMouseWheelScrolled(Arranged.GetGeometry(), ScrollDelta))
            {
                return true;
            }
        }
    }

    if (GetVisibility().IsHitTestVisible() &&
        AllottedGeometry.GetRenderBoundingRect().PtInRect(CursorPos))
    {
        return OnReceiveMouseWheelScrolled(AllottedGeometry, ScrollDelta);
    }

    return false;
}

bool SCompoundWidget::SendMouseEvent(const Geometry& AllottedGeometry, const Vector2N& Location, EMouseButton Button, EMouseButtonEvent Event)
{
    Super::SendMouseEvent(AllottedGeometry, Location, Button, Event);

    if (GetVisibility().AreChildrenHitTestVisible())
    {
        ArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
        ArrangeChildren(ArrangedChildren, AllottedGeometry);

        for (auto& Arranged : ArrangedChildren.GetWidgets())
        {
            if (Arranged.GetWidget()->SendMouseEvent(Arranged.GetGeometry(), Location, Button, Event))
            {
                return true;
            }
        }
    }

    if (GetVisibility().IsHitTestVisible() &&
        AllottedGeometry.GetRenderBoundingRect().PtInRect(Cast<Vector2>(Location)))
    {
        return OnReceiveMouseEvent(AllottedGeometry, Location, Button, Event);
    }

    return false;
}

bool SCompoundWidget::SendKeyboardEvent(const Geometry& AllottedGeometry, EKey Key, EKeyboardEvent Event)
{
    Super::SendKeyboardEvent(AllottedGeometry, Key, Event);

    if (GetVisibility().AreChildrenHitTestVisible())
    {
        ArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
        ArrangeChildren(ArrangedChildren, AllottedGeometry);

        for (auto& Arranged : ArrangedChildren.GetWidgets())
        {
            if (Arranged.GetWidget()->SendKeyboardEvent(Arranged.GetGeometry(), Key, Event))
            {
                return true;
            }
        }
    }

    if (GetVisibility().IsHitTestVisible())
    {
        return OnReceiveKeyboardEvent(AllottedGeometry, Key, Event);
    }

    return false;
}

bool SCompoundWidget::SendIMEEvent(const Geometry& AllottedGeometry, const IMEEvent& EventArgs)
{
    Super::SendIMEEvent(AllottedGeometry, EventArgs);

    if (GetVisibility().AreChildrenHitTestVisible())
    {
        ArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
        ArrangeChildren(ArrangedChildren, AllottedGeometry);

        for (auto& Arranged : ArrangedChildren.GetWidgets())
        {
            if (Arranged.GetWidget()->SendIMEEvent(Arranged.GetGeometry(), EventArgs))
            {
                return true;
            }
        }
    }

    if (GetVisibility().IsHitTestVisible())
    {
        return OnReceiveIMEEvent(AllottedGeometry, EventArgs);
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
    const PaintArgs NewArgs = Args.WithNewParent(this);
    bool bShouldBeEnabled = ShouldBeEnabled(bParentEnabled);

    for (const ArrangedWidget& Arranged : ArrangedChildrens.GetWidgets())
    {
        SWidget* CurChild = Arranged.GetWidget();

        if (!IsChildWidgetCulled(CullingRect, Arranged))
        {
            int32 CurChildsMaxLayer = CurChild->Paint(NewArgs, Arranged.GetGeometry(), CullingRect, DrawCollector, InLayer, bShouldBeEnabled);
            InLayer = MathEx::Max(CurChildsMaxLayer, InLayer);
        }
    }

    return InLayer;
}

bool SCompoundWidget::OnReceiveMouseMoved(const Geometry& AllottedGeometry, const Vector2N& Location)
{
    ArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
    ArrangeChildren(ArrangedChildren, AllottedGeometry);

    for (auto& Arranged : ArrangedChildren.GetWidgets())
    {
        if (Arranged.GetWidget()->SendMouseMoved(Arranged.GetGeometry(), Location))
        {
            return true;
        }
    }

    return false;
}

bool SCompoundWidget::OnReceiveMouseWheelScrolled(const Geometry& AllottedGeometry, int32 ScrollDelta)
{
    ArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
    ArrangeChildren(ArrangedChildren, AllottedGeometry);

    for (auto& Arranged : ArrangedChildren.GetWidgets())
    {
        if (Arranged.GetWidget()->SendMouseWheelScrolled(Arranged.GetGeometry(), ScrollDelta))
        {
            return true;
        }
    }

    return false;
}

bool SCompoundWidget::OnReceiveMouseEvent(const Geometry& AllottedGeometry, const Vector2N& Location, EMouseButton Button, EMouseButtonEvent Event)
{
    ArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
    ArrangeChildren(ArrangedChildren, AllottedGeometry);

    for (auto& Arranged : ArrangedChildren.GetWidgets())
    {
        if (Arranged.GetWidget()->SendMouseEvent(Arranged.GetGeometry(), Location, Button, Event))
        {
            return true;
        }
    }

    return false;
}

bool SCompoundWidget::OnReceiveKeyboardEvent(const Geometry& AllottedGeometry, EKey Key, EKeyboardEvent Event)
{
    ArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
    ArrangeChildren(ArrangedChildren, AllottedGeometry);

    for (auto& ArrangedWidget : ArrangedChildren.GetWidgets())
    {
        if (ArrangedWidget.GetWidget()->SendKeyboardEvent(ArrangedWidget.GetGeometry(), Key, Event))
        {
            return true;
        }
    }

    return false;
}

bool SCompoundWidget::OnReceiveIMEEvent(const Geometry& AllottedGeometry, const IMEEvent& EventArgs)
{
    ArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
    ArrangeChildren(ArrangedChildren, AllottedGeometry);

    for (auto& ArrangedWidget : ArrangedChildren.GetWidgets())
    {
        if (ArrangedWidget.GetWidget()->SendIMEEvent(ArrangedWidget.GetGeometry(), EventArgs))
        {
            return true;
        }
    }

    return false;
}