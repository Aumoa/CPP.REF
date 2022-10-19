// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/Input/SButton.h"
#include "Layout/ArrangedChildrens.h"
#include "SButton.gen.cpp"

SButton::SButton() : Super()
{
}

bool SButton::OnReceiveMouseEvent(const Geometry& AllottedGeometry, EMouseButton Button, EMouseButtonEvent Event, const MouseState& State)
{
	if (AllottedGeometry.IsUnderLocation(Vector<>::Cast<Vector2>(State.GetCursorLocation())))
	{
		if (Button == InterruptButton && Event == CaughtEvent)
		{
			ButtonClicked.Broadcast();
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