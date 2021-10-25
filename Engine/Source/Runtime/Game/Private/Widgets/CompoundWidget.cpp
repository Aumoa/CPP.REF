// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/CompoundWidget.h"
#include "Draw/SlateWindowElementList.h"
#include "Draw/PaintArgs.h"
#include "Layout/ArrangedChildrens.h"

SCompoundWidget::SCompoundWidget() : Super()
{
}

int32 SCompoundWidget::OnPaint(SPaintArgs* paintArgs, const Geometry& allottedGeometry, const Rect& cullingRect, SSlateWindowElementList* drawElements, int32 layer, bool bParentEnabled)
{
	ScopedPtr arrangedChildrens = drawElements->NewObject<SArrangedChildrens>(ESlateVisibility::Visible);
	ArrangeChildren(arrangedChildrens.Get(), allottedGeometry);

	return PaintArrangedChildrens(paintArgs, arrangedChildrens.Get(), allottedGeometry, cullingRect, drawElements, layer, bParentEnabled);
}

int32 SCompoundWidget::PaintArrangedChildrens(SPaintArgs* paintArgs, SArrangedChildrens* arrangedChildrens, const Geometry& allottedGeometry, const Rect& cullingRect, SSlateWindowElementList* drawElements, int32 layer, bool bParentEnabled)
{
    ScopedPtr newArgs = paintArgs->WithNewParent(this);
    bool shouldBeEnabled = ShouldBeEnabled(bParentEnabled);

    for (const ArrangedWidget& arrangedWidget : arrangedChildrens->GetWidgets())
    {
        SWidget* curWidget = arrangedWidget.GetWidget();

        if (!IsChildWidgetCulled(cullingRect, arrangedWidget))
        {
            int32 curWidgetsMaxLayer = curWidget->Paint(newArgs.Get(), arrangedWidget.GetGeometry(), cullingRect, drawElements, layer, shouldBeEnabled);
            layer = MathEx::Max(curWidgetsMaxLayer, layer);
        }
    }

    return layer;
}

bool SCompoundWidget::OnReceiveMouseEvent(const Geometry& allottedGeometry, const Vector2N& location, EMouseButton button, EMouseButtonEvent event)
{
    ScopedPtr arrangedChildrens = NewObject<SArrangedChildrens>(ESlateVisibility::Visible);
    ArrangeChildren(arrangedChildrens.Get(), allottedGeometry);

    for (auto& arrangedWidget : arrangedChildrens->GetWidgets())
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
    ScopedPtr arrangedChildrens = NewObject<SArrangedChildrens>(ESlateVisibility::Visible);
    ArrangeChildren(arrangedChildrens.Get(), allottedGeometry);

    for (auto& arrangedWidget : arrangedChildrens->GetWidgets())
    {
        if (arrangedWidget.GetWidget()->SendKeyboardEvent(arrangedWidget.GetGeometry(), key, event))
        {
            return true;
        }
    }

    return false;
}