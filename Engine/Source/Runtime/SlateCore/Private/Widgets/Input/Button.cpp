// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/Input/Button.h"
#include "Layout/ArrangedChildrens.h"

SButton::SButton() : Super()
{
}

bool SButton::OnReceiveMouseEvent(const Geometry& AllottedGeometry, const Vector2N& Location, EMouseButton Button, EMouseButtonEvent Event)
{
	if (AllottedGeometry.IsUnderLocation(Location.Cast<float>()))
	{
		if (Button == InterruptButton && Event == CaughtEvent)
		{
			ButtonClicked.Invoke();
			return true;
		}
	}

	return false;
}

DEFINE_SLATE_CONSTRUCTOR(SButton, Attr)
{
	INVOKE_SLATE_CONSTRUCTOR_SUPER(Attr);
	InterruptButton = Attr._InterruptButton;
	CaughtEvent = Attr._CaughtEvent;
}