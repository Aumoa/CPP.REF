// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "LayoutImpl.h"
#include "Geometry.h"
#include "ArrangedWidget.h"

ArrangedWidget Geometry::MakeChild(const SWidget* childWidget, const Vector2& localSize, const SlateLayoutTransform& layoutTransform) const
{
	checkf(false, L"NOT IMPLEMENTED");
	throw;
}

ArrangedWidget Geometry::MakeChild(const SWidget* childWidget, const LayoutGeometry& layoutGeometry) const
{
	return MakeChild(childWidget, layoutGeometry.GetSizeInLocalSpace(), layoutGeometry.GetLocalToParentTransform());
}

ArrangedWidget Geometry::MakeChild(const SWidget* childWidget, const Vector2& childOffset, const Vector2& localSize, float childScale) const
{
	return MakeChild(childWidget, localSize, SlateLayoutTransform(childScale, TransformCalculus2D::TransformPoint(childScale, childOffset)));
}