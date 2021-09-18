// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Widgets/CompoundWidget.h"
#include "Draw/SlateWindowElementList.h"
#include "Draw/PaintArgs.h"
#include "Layout/ArrangedChildrens.h"

SCompoundWidget::SCompoundWidget(const std::wstring& name) : Super(name)
{
}

int32 SCompoundWidget::OnPaint(SPaintArgs* paintArgs, const Geometry& allottedGeometry, const Rect& cullingRect, SSlateWindowElementList* drawElements, int32 layer, bool bParentEnabled) const
{
	ScopedPtr arrangedChildrens = drawElements->NewObject<SArrangedChildrens>(ESlateVisibility::Visible);
	ArrangeChildren(arrangedChildrens.Get(), allottedGeometry);

	return PaintArrangedChildrens(paintArgs, arrangedChildrens.Get(), allottedGeometry, cullingRect, drawElements, layer, bParentEnabled);
}

int32 SCompoundWidget::PaintArrangedChildrens(SPaintArgs* paintArgs, SArrangedChildrens* arrangedChildrens, const Geometry& allottedGeometry, const Rect& cullingRect, SSlateWindowElementList* drawElements, int32 layer, bool bParentEnabled) const
{
    ScopedPtr newArgs = paintArgs->WithNewParent(this);
    bool shouldBeEnabled = ShouldBeEnabled(bParentEnabled);

    for (const ArrangedWidget& arrangedWidget : arrangedChildrens->GetWidgets())
    {
        const SWidget* curWidget = arrangedWidget.GetWidget();

        if (!IsChildWidgetCulled(cullingRect, arrangedWidget))
        {
            int32 curWidgetsMaxLayer = curWidget->Paint(newArgs.Get(), arrangedWidget.GetGeometry(), cullingRect, drawElements, layer, shouldBeEnabled);
            layer = MathEx::Max(curWidgetsMaxLayer, layer);
        }
    }

    return layer;
}