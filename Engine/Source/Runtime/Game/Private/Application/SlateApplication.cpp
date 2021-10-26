// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Application/SlateApplication.h"
#include "Widgets/Window.h"
#include "Draw/PaintArgs.h"
#include "Draw/SlateWindowElementList.h"
#include "SceneRendering/SlateRenderer.h"
#include "EngineSubsystems/GameRenderSystem.h"
#include "RHI/IRHIDevice.h"
#include "RHI/IRHIDeviceContext.h"

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

	if (DeferredContext == nullptr)
	{
		DeferredContext = Args.DeviceContext->GetDevice()->CreateDeviceContext();
		DeferredContext->SetOuter(this);
	}

	SSlateShader* Shader = GEngine->GetEngineSubsystem<SGameRenderSystem>()->GetSlateShader();
	auto InitContext = Shader->InitElements(Elements);

	DeferredContext->Begin(InitContext.NumDescriptors, 0);

	SlateRenderer Renderer(*Args.RenderTarget, Shader, &InitContext);
	Renderer.PopulateCommandLists(DeferredContext);

	DeferredContext->End();
	Args.DeviceContext->ExecuteCommandList(DeferredContext);
}