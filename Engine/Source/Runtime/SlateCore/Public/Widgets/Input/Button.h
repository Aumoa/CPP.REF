// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Layout/Border.h"

class SLATECORE_API SButton : public SBorder
{
	GENERATED_BODY(SButton)

public:
	SButton();

protected:
	virtual bool OnReceiveMouseEvent(const Geometry& AllottedGeometry, const Vector2N& Location, EMouseButton Button, EMouseButtonEvent Event) override;

public:
	DECLARE_MULTICAST_EVENT(ButtonClickedEvent, EMouseButton, EMouseButtonEvent);
	ButtonClickedEvent ButtonClicked;
};