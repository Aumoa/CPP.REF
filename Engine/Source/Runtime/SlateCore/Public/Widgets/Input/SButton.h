// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Layout/SBorder.h"
#include "SButton.gen.h"

SCLASS()
class GAME_API SButton : public SBorder
{
	GENERATED_BODY()

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