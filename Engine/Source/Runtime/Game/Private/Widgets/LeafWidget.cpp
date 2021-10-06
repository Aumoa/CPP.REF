// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/LeafWidget.h"
#include "Layout/LayoutImpl.h"

SLeafWidget::SLeafWidget(const std::wstring& name) : Super(name)
{
}

SLeafWidget::~SLeafWidget()
{
}

void SLeafWidget::OnArrangeChildren(SArrangedChildrens* arrangedChildrens, const Geometry& allottedGeometry)
{
}

bool SLeafWidget::OnReceiveMouseEvent(const Geometry&, const Vector2N&, EMouseButton, EMouseButtonEvent)
{
	return false;
}

bool SLeafWidget::OnReceiveKeyboardEvent(const Geometry&, EKey, EKeyboardEvent)
{
	return false;
}