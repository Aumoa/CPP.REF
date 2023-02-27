// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "SlateApplication.h"
#include "Widgets/Viewports/SWindow.h"
#include "Draw/PaintArgs.h"

using namespace ::libty;

SSlateApplication::SSlateApplication()
{
}

void SSlateApplication::Inject(SRenderEngine* REngine)
{
	RContext = REngine->CreateRenderContext();
	CoreWindow = SNew(SWindow)
		.BufferCount(3);
}

void SSlateApplication::TickAndPaint(float InDeltaTime)
{
	IApplicationInterface& AppInstance = IApplicationInterface::Get();
	Vector2N VpSize = AppInstance.GetViewportSize();

	Geometry AppGeometry = Geometry::MakeRoot(VpSize, SlateLayoutTransform::Identity());
	CoreWindow->Tick(AppGeometry, InDeltaTime);

	PaintArgs InitArgs = PaintArgs::InitPaintArgs(RContext, InDeltaTime);
	CoreWindow->Paint(InitArgs, AppGeometry, AppGeometry.GetRenderBoundingRect(), nullptr, 0, true);
}

void SSlateApplication::RenderApp(SRenderContext* RContext)
{
	CoreWindow->RenderWindow(RContext);
}