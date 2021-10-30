// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/LeafWidget.h"

SLeafWidget::SLeafWidget() : Super()
{
}

void SLeafWidget::OnArrangeChildren(ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry)
{
}

bool SLeafWidget::OnReceiveMouseEvent(const Geometry& AllottedGeometry, const Vector2N& Location, EMouseButton Button, EMouseButtonEvent Event)
{
	return false;
}

bool SLeafWidget::OnReceiveKeyboardEvent(const Geometry& AllottedGeometry, EKey Key, EKeyboardEvent Event)
{
	return false;
}