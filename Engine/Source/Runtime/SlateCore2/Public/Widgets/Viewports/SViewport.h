// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SLeafWidget.h"
#include "RHI/RHIRaytracingRenderTarget.h"
#include "SViewport.gen.h"

class SWindow;
class RHIResource;
struct SceneRenderContext;

SCLASS()
class GAME_API SViewport : public SLeafWidget
{
	GENERATED_BODY()

private:
	std::shared_ptr<RHIDevice> Device;
	RHIRaytracingRenderTarget RenderTarget;

	std::shared_ptr<RHIDescriptorTable> DescriptorTable;

public:
	SViewport();

	virtual void Tick(const Geometry& AllottedGeometry, const TimeSpan& DeltaTime) override;
	virtual int32 OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SSlateDrawCollector* DrawCollector, int32 InLayer, bool bParentEnabled) override;

	std::shared_ptr<RHIResource> PresentViewport(SceneRenderContext& Context, RHICommandList* CmdList);

public:
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_ATTRIBUTE(SWindow*, Window)
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();

private:
	void TryResizeViewport(const Geometry& AllottedGeometry);
	void ComposeRaytracingShaders();
};