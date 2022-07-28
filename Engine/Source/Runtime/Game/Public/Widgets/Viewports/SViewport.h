// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Widgets/SLeafWidget.h"

class SWindow;

class GAME_API SViewport : public SLeafWidget
{
	using This = SViewport;
	using Super = SLeafWidget;

public:
	SViewport();

	virtual void Tick(const Geometry& AllottedGeometry, const TimeSpan& deltaTime) override;
	virtual int32 OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SSlateDrawCollector* DrawCollector, int32 InLayer, bool bParentEnabled) override;

public:
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_ATTRIBUTE(SWindow*, Window)
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();

private:
	void OnSwapChainResized_RenderThread(Vector2N size);
};