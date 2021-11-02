// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Application/SlateApplication.h"
#include "Widgets/Window.h"
#include "Draw/PaintArgs.h"
#include "Draw/SlateWindowElementList.h"
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

	if (DeviceContext == nullptr)
	{
		auto Dev = GEngine->GetEngineSubsystem<SGameRenderSystem>()->GetRHIDevice();
		DeviceContext = Dev->CreateDeviceContext();
	}

	//RenderThread::EnqueueRenderThreadWork<"TickAndPaint_InitContexts">([&, Elements = std::move(Elements)](auto)
	//{
	//	SSlateShader* Shader = GEngine->GetEngineSubsystem<SGameRenderSystem>()->GetSlateShader();
	//	InitContext_RenderThread = Shader->InitElements(*Elements);
	//});
}

void SSlateApplication::PopulateCommandLists(SceneRenderContext& RenderContext)
{
	//SSlateShader* Shader = GEngine->GetEngineSubsystem<SGameRenderSystem>()->GetSlateShader();

	//IRHIDeviceContext* DeviceContext = RenderContext.DeviceContext;
	//DeviceContext->SetDescriptorHeaps(InitContext_RenderThread.NumDescriptors, 0);

	//SlateRenderer Renderer(*RenderContext.RenderTarget, Shader, &InitContext_RenderThread);
	//Renderer.PopulateCommandLists(DeviceContext);
}

SLocalPlayer* SSlateApplication::GetLocalPlayer()
{
	return LocalPlayer;
}