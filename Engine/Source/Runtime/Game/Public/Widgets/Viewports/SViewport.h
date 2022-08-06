// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Widgets/SLeafWidget.h"
#include "RHI/RHIRaytracingRenderTarget.h"

class SWindow;
class RHIResource;

class GAME_API SViewport : public SLeafWidget
{
	using This = SViewport;
	using Super = SLeafWidget;

private:
	std::shared_ptr<RHIDevice> _device;
	RHIRaytracingRenderTarget _renderTarget;

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
	void TryResizeViewport(const Geometry& allottedGeometry);
};