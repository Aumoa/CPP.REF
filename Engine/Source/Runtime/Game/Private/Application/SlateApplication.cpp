// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Application/SlateApplication.h"
#include "Application/Window.h"
#include "Draw/PaintArgs.h"
#include "Draw/SlateRenderer.h"
#include "Draw/SlateDrawCollector.h"
#include "Draw/IRenderSlateElement.h"
#include "SceneRendering/SceneRenderContext.h"
#include "EngineSubsystems/GameRenderSystem.h"
#include "RHI/IRHIDevice.h"
#include "RHI/IRHIDeviceContext.h"
#include "GameFramework/LocalPlayer.h"
#include "Input/IPlatformKeyboard.h"
#include "Input/IPlatformMouse.h"
#include "RenderThread.h"
#include "IApplicationInterface.h"

GENERATE_BODY(SSlateApplication);

SSlateApplication::SSlateApplication() : Super()
{
}

SSlateApplication::~SSlateApplication()
{
	// Remove all delegates for dispose DLL link.
	auto& App = IApplicationInterface::Get();

	auto& PlatformKeyboard = App.GetPlatformKeyboard();
	PlatformKeyboard.KeyPressed.RemoveAll(this);
	PlatformKeyboard.KeyReleased.RemoveAll(this);

	auto& PlatformMouse = App.GetPlatformMouse();
	PlatformMouse.CursorMoved.RemoveAll(this);
	PlatformMouse.MouseButtonPressed.RemoveAll(this);
	PlatformMouse.MouseButtonReleased.RemoveAll(this);
	PlatformMouse.MouseWheelScrolled.RemoveAll(this);
}

void SSlateApplication::Init(IApplicationInterface* InApplication)
{
	LocalPlayer = gcnew SLocalPlayer(this);
	DrawCollector = gcnew SSlateDrawCollector();

	CoreWindow = CreateCoreWindow();
	CoreWindow->InitViewport();

	auto& PlatformKeyboard = InApplication->GetPlatformKeyboard();
	PlatformKeyboard.KeyPressed.AddSObject(this, &SSlateApplication::OnPlatformKeyPressed);
	PlatformKeyboard.KeyReleased.AddSObject(this, &SSlateApplication::OnPlatformKeyReleased);

	auto& PlatformMouse = InApplication->GetPlatformMouse();
	PlatformMouse.CursorMoved.AddSObject(this, &SSlateApplication::OnCursorMoved);
	PlatformMouse.MouseButtonPressed.AddSObject(this, &SSlateApplication::OnMouseButtonPressed);
	PlatformMouse.MouseButtonReleased.AddSObject(this, &SSlateApplication::OnMouseButtonReleased);
	PlatformMouse.MouseWheelScrolled.AddSObject(this, &SSlateApplication::OnMouseWheelScrolled);
}

void SSlateApplication::TickAndPaint(float InDeltaTime)
{
	if (Device == nullptr)
	{
		Device = GEngine->GetEngineSubsystem<SGameRenderSystem>()->GetRHIDevice();
	}

	Vector2 DesiredSize = IApplicationInterface::Get().GetViewportSize().Cast<float>();
	Geometry AllottedGeometry = Geometry::MakeRoot(DesiredSize, SlateLayoutTransform(Vector2::ZeroVector()), SlateRenderTransform(Vector2::ZeroVector()));
	CoreWindow->Tick(AllottedGeometry, InDeltaTime);

	Rect CullingRect = Rect(0, 0, DesiredSize.X, DesiredSize.Y);
	CoreWindow->Paint(PaintArgs::InitPaintArgs(Device, InDeltaTime), AllottedGeometry, CullingRect, DrawCollector, 0, true);

	DrawCollector->SortByLayer();
	CachedElements.clear();
	DrawCollector->FlushElements(CachedElements);

	RenderThread::EnqueueRenderThreadWork<"TickAndPaint">([this, Buf = CachedElements](auto) mutable
	{
		RenderElements = std::move(Buf);
	});
}

void SSlateApplication::DrawElements(IRHIDeviceContext2D* CommandList, SSlateRenderer* Renderer)
{
	Renderer->PopulateCommands(CommandList, RenderElements);
}

SLocalPlayer* SSlateApplication::GetLocalPlayer()
{
	return LocalPlayer;
}

SWindow* SSlateApplication::GetCoreWindow()
{
	return CoreWindow;
}

SWindow* SSlateApplication::CreateCoreWindow()
{
	return SNew(SWindow);
}

void SSlateApplication::OnPlatformKeyPressed(EKey InKey)
{
	CoreWindow->SendKeyboardEvent(MakeRoot(), InKey, EKeyboardEvent::Pressed);
}

void SSlateApplication::OnPlatformKeyReleased(EKey InKey)
{
	CoreWindow->SendKeyboardEvent(MakeRoot(), InKey, EKeyboardEvent::Released);
}

void SSlateApplication::OnCursorMoved(Vector2N Location)
{
	CoreWindow->SendMouseMoved(MakeRoot(), Location);
}

void SSlateApplication::OnMouseButtonPressed(Vector2N Location, EMouseButton Button)
{
	CoreWindow->SendMouseEvent(MakeRoot(), Location, Button, EMouseButtonEvent::Pressed);
}

void SSlateApplication::OnMouseButtonReleased(Vector2N Location, EMouseButton Button)
{
	CoreWindow->SendMouseEvent(MakeRoot(), Location, Button, EMouseButtonEvent::Released);
}

void SSlateApplication::OnMouseWheelScrolled(int32 ScrollDelta)
{
	CoreWindow->SendMouseWheelScrolled(MakeRoot(), ScrollDelta);
}

Geometry SSlateApplication::MakeRoot()
{
	Vector2 DesiredSize = CoreWindow->GetDesiredSize();
	Geometry AllottedGeometry = Geometry::MakeRoot(DesiredSize, SlateLayoutTransform(Vector2::ZeroVector()), SlateRenderTransform(Vector2::ZeroVector()));
	return AllottedGeometry;
}