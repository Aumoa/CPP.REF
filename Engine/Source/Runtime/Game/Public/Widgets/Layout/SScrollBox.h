// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Widgets/Panel/SBoxPanel.h"

class GAME_API SScrollBox : public SBoxPanel
{
	using This = SScrollBox;
	using Super = SBoxPanel;

private:
	float ScrollAmount = 0;

public:
	SScrollBox();

	virtual void Tick(const Geometry& AllottedGeometry, float InDeltaTime);
	virtual bool OnReceiveMouseWheelScrolled(const Geometry& AllottedGeometry, int32 ScrollDelta, const MouseState& State) override;

	void ScrollToBottom();

protected:
	virtual void OnArrangeChildren(ArrangedChildrens& ArrangedChildrens, const Geometry& AllottedGeometry) override;

public:
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_ATTRIBUTE(EOrientation, Orientation, EOrientation::Vertical)
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();
};