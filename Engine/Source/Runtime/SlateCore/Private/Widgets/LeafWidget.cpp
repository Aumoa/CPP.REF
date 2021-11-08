// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/LeafWidget.h"
#include "Layout/LayoutImpl.h"

SLeafWidget::SLeafWidget() : Super()
{
}

SLeafWidget::~SLeafWidget()
{
}

void SLeafWidget::OnArrangeChildren(ArrangedChildrens& ArrangedChildrens, const Geometry& AllottedGeometry)
{
}

bool SLeafWidget::OnReceiveMouseMoved(const Geometry& AllottedGeometry, const Vector2N& Location)
{
    return false;
}

bool SLeafWidget::OnReceiveMouseWheelScrolled(const Geometry& AllottedGeometry, int32 ScrollDelta)
{
    return false;
}

bool SLeafWidget::OnReceiveMouseEvent(const Geometry& AllottedGeometry, const Vector2N& Location, EMouseButton Button, EMouseButtonEvent Event)
{
	return false;
}

bool SLeafWidget::OnReceiveKeyboardEvent(const Geometry& AllottedGeometry, EKey Key, EKeyboardEvent Event)
{
    return false;
}