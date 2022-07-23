// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Widgets/Layout/SBorder.h"

class GAME_API SButton : public SBorder
{
	using This = SButton;
	using Super = SBorder;

public:
	SButton();

protected:
	virtual bool OnReceiveMouseEvent(const Geometry& AllottedGeometry, EMouseButton Button, EMouseButtonEvent Event, const MouseState& State) override;

public:
	DECLARE_MULTICAST_EVENT(ButtonClickedEvent);
	ButtonClickedEvent ButtonClicked;

private:
	EMouseButton InterruptButton;
	EMouseButtonEvent CaughtEvent;

public:
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_ATTRIBUTE(EMouseButton, InterruptButton, EMouseButton::Left)
		DECLARE_SLATE_ATTRIBUTE(EMouseButtonEvent, CaughtEvent, EMouseButtonEvent::Released)
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();
};