// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Layout/LayoutImpl.h"
#include "Layout/Geometry.h"
#include "Layout/ArrangedWidget.h"
#include "Widgets/Widget.h"

ArrangedWidget Geometry::MakeChild(SWidget* ChildWidget, const Vector2& LocalSize, const SlateLayoutTransform& LayoutTransform) const
{
    if (ChildWidget->HasRenderTransform())
    {
        SlateRenderTransform RenderTransform = ChildWidget->GetRenderTransformWithRespectToFlowDirection();
        Vector2 RenderTransformPivot = ChildWidget->GetRenderTransformPivotWithRespectToFlowDirection();
        return ArrangedWidget(ChildWidget, MakeChild(LocalSize, LayoutTransform, RenderTransform, RenderTransformPivot));
    }
    else
    {
        return ArrangedWidget(ChildWidget, MakeChild(LocalSize, LayoutTransform));
    }
}

ArrangedWidget Geometry::MakeChild(SWidget* ChildWidget, const LayoutGeometry& LayoutGeometry) const
{
	return MakeChild(ChildWidget, LayoutGeometry.GetSizeInLocalSpace(), LayoutGeometry.GetLocalToParentTransform());
}

ArrangedWidget Geometry::MakeChild(SWidget* ChildWidget, const Vector2& ChildOffset, const Vector2& LocalSize, float ChildScale) const
{
	return MakeChild(ChildWidget, LocalSize, SlateLayoutTransform(ChildScale, TransformCalculus2D::TransformPoint(ChildScale, ChildOffset)));
}