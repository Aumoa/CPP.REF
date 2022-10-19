// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Panel/SBoxPanel.h"
#include "SScrollBox.gen.h"

SCLASS()
class GAME_API SScrollBox : public SBoxPanel
{
	GENERATED_BODY()

private:
	float ScrollAmount = 0;

public:
	SScrollBox();

	virtual void Tick(const Geometry& AllottedGeometry, const TimeSpan& deltaTime);
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