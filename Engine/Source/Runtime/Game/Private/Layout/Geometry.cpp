// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Layout/LayoutImpl.h"
#include "Layout/Geometry.h"
#include "Layout/ArrangedWidget.h"
#include "Widgets/Widget.h"

ArrangedWidget Geometry::MakeChild(SWidget* childWidget, const Vector2& localSize, const SlateLayoutTransform& layoutTransform) const
{
    if (childWidget->HasRenderTransform())
    {
        SlateRenderTransform renderTransform = childWidget->GetRenderTransformWithRespectToFlowDirection();
        Vector2 renderTransformPivot = childWidget->GetRenderTransformPivotWithRespectToFlowDirection();
        return ArrangedWidget(childWidget, MakeChild(localSize, layoutTransform, renderTransform, renderTransformPivot));
    }
    else
    {
        return ArrangedWidget(childWidget, MakeChild(localSize, layoutTransform));
    }
}

ArrangedWidget Geometry::MakeChild(SWidget* childWidget, const LayoutGeometry& layoutGeometry) const
{
	return MakeChild(childWidget, layoutGeometry.GetSizeInLocalSpace(), layoutGeometry.GetLocalToParentTransform());
}

ArrangedWidget Geometry::MakeChild(SWidget* childWidget, const Vector2& childOffset, const Vector2& localSize, float childScale) const
{
	return MakeChild(childWidget, localSize, SlateLayoutTransform(childScale, TransformCalculus2D::TransformPoint(childScale, childOffset)));
}