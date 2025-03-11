// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Layout/Geometry.h"
#include "Layout/ArrangedWidget.h"
#include "Widgets/SWidget.h"

NArrangedWidget NGeometry::MakeChild(std::shared_ptr<SWidget> ChildWidget, const Vector2& LocalSize, const NSlateLayoutTransform& LayoutTransform) const
{
    if (ChildWidget->HasRenderTransform())
    {
        NSlateRenderTransform RenderTransform = ChildWidget->GetRenderTransformWithRespectToFlowDirection();
        Vector2 RenderTransformPivot = ChildWidget->GetRenderTransformPivotWithRespectToFlowDirection();
        return NArrangedWidget(ChildWidget, MakeChild(LocalSize, LayoutTransform, RenderTransform, RenderTransformPivot));
    }
    else
    {
        return NArrangedWidget(ChildWidget, MakeChild(LocalSize, LayoutTransform));
    }
}

NArrangedWidget NGeometry::MakeChild(std::shared_ptr<SWidget> ChildWidget, const NLayoutGeometry& LayoutGeometry) const
{
	return MakeChild(ChildWidget, LayoutGeometry.GetSizeInLocalSpace(), LayoutGeometry.GetLocalToParentTransform());
}

NArrangedWidget NGeometry::MakeChild(std::shared_ptr<SWidget> ChildWidget, const Translate2D& ChildOffset, const Vector2& LocalSize, const Scale2D& ChildScale) const
{
	return MakeChild(ChildWidget, LocalSize, NSlateLayoutTransform(ChildScale, ChildScale.TransformPoint(ChildOffset)));
}