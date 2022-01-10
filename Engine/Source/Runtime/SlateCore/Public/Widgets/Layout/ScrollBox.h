// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Panel/BoxPanel.h"

class SLATECORE_API SScrollBox : public SBoxPanel
{
	GENERATED_BODY(SScrollBox)

private:
	float ScrollAmount = 0;

public:
	SScrollBox();

	virtual void Tick(const Geometry& AllottedGeometry, float InDeltaTime);
	virtual bool OnReceiveMouseWheelScrolled(const Geometry& AllottedGeometry, int32 ScrollDelta) override;

	void ScrollToBottom();

protected:
	virtual void OnArrangeChildren(ArrangedChildrens& ArrangedChildrens, const Geometry& AllottedGeometry) override;

public:
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_ATTRIBUTE(EOrientation, Orientation, EOrientation::Vertical)
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();
};