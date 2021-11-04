// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Application/SlateApplication.h"
#include "Application/Window.h"
#include "Draw/PaintArgs.h"
#include "Draw/SlateWindowElementList.h"
#include "Draw/SlateRenderer.h"
#include "SceneRendering/SlateRenderer.h"
#include "SceneRendering/SceneRenderContext.h"
#include "EngineSubsystems/GameRenderSystem.h"
#include "RHI/IRHIDevice.h"
#include "RHI/IRHIDeviceContext.h"
#include "RenderThread.h"
#include "GameFramework/LocalPlayer.h"

SSlateApplication::SSlateApplication() : Super()
{
}

SSlateApplication::~SSlateApplication()
{
}

void SSlateApplication::Init(IApplicationInterface* InApplication)
{
	LocalPlayer = NewObject<SLocalPlayer>();
	(SAssignNew(CoreWindow, SWindow))->InitViewport(InApplication);
}

void SSlateApplication::TickAndPaint(float InDeltaTime)
{
	Vector2 DesiredSize = CoreWindow->GetDesiredSize();
	Geometry AllottedGeometry = Geometry::MakeRoot(DesiredSize, SlateLayoutTransform(Vector2::ZeroVector()), SlateRenderTransform(Vector2::ZeroVector()));
	CoreWindow->Tick(AllottedGeometry, InDeltaTime);

	Rect CullingRect = Rect(0, 0, DesiredSize.X, DesiredSize.Y);
	auto Elements = std::make_shared<SlateWindowElementList>(CoreWindow);
	CoreWindow->Paint(PaintArgs(nullptr, InDeltaTime), AllottedGeometry, CullingRect, *Elements, 0, true);

	RenderThread::EnqueueRenderThreadWork<"TickAndPaint">([this, Buf = std::move(Elements)](auto)
	{
		SlateElements = std::move(Buf);
	});
}

void SSlateApplication::DrawElements(SlateRenderer* Renderer)
{
	Renderer->PopulateCommands(*SlateElements);
}

SLocalPlayer* SSlateApplication::GetLocalPlayer()
{
	return LocalPlayer;
}

SWindow* SSlateApplication::GetCoreWindow()
{
	return CoreWindow;
}