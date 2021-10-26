// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widget.h"

class GAME_API SLeafWidget : public SWidget
{
	GENERATED_BODY(SLeafWidget)

public:
	SLeafWidget();
	virtual ~SLeafWidget() override;

protected:
	virtual void OnArrangeChildren(ArrangedChildrens& ArrangedChildrens, const Geometry& AllottedGeometry) override;

	virtual bool OnReceiveMouseEvent(const Geometry& allottedGeometry, const Vector2N& location, EMouseButton button, EMouseButtonEvent event) override;
	virtual bool OnReceiveKeyboardEvent(const Geometry& allottedGeometry, EKey key, EKeyboardEvent event) override;
};