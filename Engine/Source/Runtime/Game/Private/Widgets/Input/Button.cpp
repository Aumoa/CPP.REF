// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/Input/Button.h"
#include "Layout/ArrangedChildrens.h"

SButton::SButton(const std::wstring& name) : Super(name)
{
}

bool SButton::OnReceiveMouseEvent(const Geometry& allottedGeometry, const Vector2N& location, EMouseButton button, EMouseButtonEvent event)
{
	if (allottedGeometry.IsUnderLocation(location.Cast<float>()))
	{
		ButtonClicked.Invoke(button, event);
		return true;
	}

	return false;
}