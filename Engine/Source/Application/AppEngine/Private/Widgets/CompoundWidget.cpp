// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/CompoundWidget.h"
#include "Draw/PaintArgs.h"
#include "Layout/ArrangedChildrens.h"

SCompoundWidget::SCompoundWidget() : Super()
{
}

int32 SCompoundWidget::OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SlateWindowElementList& InDrawElements, int32 InLayer, bool bParentEnabled)
{
	ArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
	ArrangeChildren(ArrangedChildren, AllottedGeometry);

	return PaintArrangedChildrens(Args, ArrangedChildren, AllottedGeometry, CullingRect, InDrawElements, InLayer, bParentEnabled);
}

int32 SCompoundWidget::PaintArrangedChildrens(const PaintArgs& Args, ArrangedChildrens& ArrangedChildrens, const Geometry& AllottedGeometry, const Rect& CullingRect, SlateWindowElementList& InDrawElements, int32 InLayer, bool bParentEnabled)
{
    PaintArgs NewArgs = Args.WithNewParent(this);
    bool bShouldBeEnabled = ShouldBeEnabled(bParentEnabled);

    for (const ArrangedWidget& Widget : ArrangedChildrens.GetWidgets())
    {
        SWidget* CurWidget = Widget.GetWidget();

        if (!IsChildWidgetCulled(CullingRect, Widget))
        {
            int32 CurWidgetsMaxLayer = CurWidget->Paint(NewArgs, Widget.GetGeometry(), CullingRect, InDrawElements, InLayer, bShouldBeEnabled);
            InLayer = MathEx::Max(CurWidgetsMaxLayer, InLayer);
        }
    }

    return InLayer;
}

bool SCompoundWidget::OnReceiveMouseEvent(const Geometry& AllottedGeometry, const Vector2N& Location, EMouseButton Button, EMouseButtonEvent Event)
{
    ArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
    ArrangeChildren(ArrangedChildren, AllottedGeometry);

    for (auto& Widget : ArrangedChildren.GetWidgets())
    {
        if (Widget.GetWidget()->SendMouseEvent(Widget.GetGeometry(), Location, Button, Event))
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