// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Layout/Border.h"

class GAME_API SButton : public SBorder
{
	GENERATED_BODY(SButton)

public:
	SButton();

protected:
	virtual bool OnReceiveMouseEvent(const Geometry& AllottedGeometry, const Vector2N& Location, EMouseButton Button, EMouseButtonEvent Event) override;

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