// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Application/SlateApplication.h"
#include "Widgets/Window.h"
#include "Draw/PaintArgs.h"
#include "Draw/SlateWindowElementList.h"
#include "SceneRendering/SlateRenderer.h"
#include "EngineSubsystems/GameRenderSystem.h"

SSlateApplication::SSlateApplication() : Super()
{
}

void SSlateApplication::InitWindow(SLocalPlayer* InLocalPlayer, IFrameworkView* InFrameworkView)
{
	LocalPlayer = InLocalPlayer;
	(CoreWindow = SNew(SWindow))->InitViewport(InFrameworkView);
}

void SSlateApplication::Tick(float InDeltaTime)
{
	Vector2 DesiredSize = CoreWindow->GetDesiredSize();
	Geometry AllottedGeometry = Geometry::MakeRoot(DesiredSize, SlateLayoutTransform(Vector2::GetZero()), SlateRenderTransform(Vector2::GetZero()));
	CoreWindow->Tick(AllottedGeometry, InDeltaTime);
}

void SSlateApplication::PopulateCommandLists(const PaintArgs& Args)
{
	Vector2 DesiredSize = CoreWindow->GetDesiredSize();
	Geometry AllottedGeometry = Geometry::MakeRoot(DesiredSize, SlateLayoutTransform(Vector2::GetZero()), SlateRenderTransform(Vector2::GetZero()));
	Rect CullingRect = Rect(0, 0, DesiredSize.X, DesiredSize.Y);

	SlateWindowElementList Elements(CoreWindow);
	CoreWindow->Paint(Args, AllottedGeometry, CullingRect, Elements, 0, true);

	SlateRenderer Renderer(*Args.RenderTarget, GEngine->GetEngineSubsystem<SGameRenderSystem>()->GetSlateShader(), Elements);
	Renderer.PopulateCommandLists(Args.DeviceContext);
}