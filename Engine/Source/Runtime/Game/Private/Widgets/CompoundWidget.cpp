// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/CompoundWidget.h"
#include "Draw/SlateWindowElementList.h"
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
    bool shouldBeEnabled = ShouldBeEnabled(bParentEnabled);

    for (const ArrangedWidget& arrangedWidget : ArrangedChildrens.GetWidgets())
    {
        SWidget* curWidget = arrangedWidget.GetWidget();

        if (!IsChildWidgetCulled(CullingRect, arrangedWidget))
        {
            int32 curWidgetsMaxLayer = curWidget->Paint(NewArgs, arrangedWidget.GetGeometry(), CullingRect, InDrawElements, InLayer, shouldBeEnabled);
            InLayer = MathEx::Max(curWidgetsMaxLayer, InLayer);
        }
    }

    return InLayer;
}

bool SCompoundWidget::OnReceiveMouseEvent(const Geometry& allottedGeometry, const Vector2N& location, EMouseButton button, EMouseButtonEvent event)
{
    ArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
    ArrangeChildren(ArrangedChildren, allottedGeometry);

    for (auto& arrangedWidget : ArrangedChildren.GetWidgets())
    {
        if (arrangedWidget.GetWidget()->SendMouseEvent(arrangedWidget.GetGeometry(), location, button, event))
        {
            return true;
        }
    }

    return false;
}

bool SCompoundWidget::OnReceiveKeyboardEvent(const Geometry& allottedGeometry, EKey key, EKeyboardEvent event)
{
    ArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
    ArrangeChildren(ArrangedChildren, allottedGeometry);

    for (auto& ArrangedWidget : ArrangedChildren.GetWidgets())
    {
        if (ArrangedWidget.GetWidget()->SendKeyboardEvent(ArrangedWidget.GetGeometry(), key, event))
        {
            return true;
        }
    }

    return false;
}