// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Widgets/CompoundWidget.h"
#include "Draw/SlateWindowElementList.h"
#include "Draw/PaintArgs.h"
#include "Layout/ArrangedChildrens.h"

SCompoundWidget::SCompoundWidget() : Super()
{
}

int32 SCompoundWidget::OnPaint(PaintArgs* paintArgs, const Geometry& allottedGeometry, const Rect& cullingRect, SlateWindowElementList* drawElements, int32 layer, bool bParentEnabled) const
{
	auto* arrangedChildrens = drawElements->NewObject<ArrangedChildrens>(ESlateVisibility::Visible);
	ArrangeChildren(arrangedChildrens, allottedGeometry);

	return PaintArrangedChildrens(paintArgs, arrangedChildrens, allottedGeometry, cullingRect, drawElements, layer, bParentEnabled);
}

int32 SCompoundWidget::PaintArrangedChildrens(PaintArgs* paintArgs, ArrangedChildrens* arrangedChildrens, const Geometry& allottedGeometry, const Rect& cullingRect, SlateWindowElementList* drawElements, int32 layer, bool bParentEnabled) const
{
    PaintArgs* newArgs = paintArgs->WithNewParent(this);
    bool shouldBeEnabled = ShouldBeEnabled(bParentEnabled);

    for (const ArrangedWidget& arrangedWidget : arrangedChildrens->GetWidgets())
    {
        const SWidget* curWidget = arrangedWidget.GetWidget();

        if (!IsChildWidgetCulled(cullingRect, arrangedWidget))
        {
            int32 curWidgetsMaxLayer = curWidget->Paint(newArgs, arrangedWidget.GetGeometry(), cullingRect, drawElements, layer, shouldBeEnabled);
            layer = MathEx::Max(curWidgetsMaxLayer, layer);
        }
    }

    return layer;
}