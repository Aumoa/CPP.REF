// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/CompoundWidget.h"
#include "Draw/PaintArgs.h"
#include "Layout/ArrangedChildrens.h"

GENERATE_BODY(SCompoundWidget);

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