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
#include "IApplicationInterface.h"
#include "GameFramework/LocalPlayer.h"
#include "Input/IPlatformKeyboard.h"
#include "Input/IPlatformMouse.h"

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

void SSlateApplication::OnPlatformKeyPressed(EKey InKey)
{
	std::optional<Geometry> CachedGeometry = CoreWindow->GetCachedGeometry();
	if (!CachedGeometry.has_value())
	{
		Vector2 DesiredSize = CoreWindow->GetDesiredSize();
		CachedGeometry = Geometry::MakeRoot(DesiredSize, SlateLayoutTransform(Vector2::ZeroVector()), SlateRenderTransform(Vector2::ZeroVector()));
	}

	CoreWindow->SendKeyboardEvent(*CachedGeometry, InKey, EKeyboardEvent::Pressed);
}

void SSlateApplication::OnPlatformKeyReleased(EKey InKey)
{
	std::optional<Geometry> CachedGeometry = CoreWindow->GetCachedGeometry();
	if (!CachedGeometry.has_value())
	{
		Vector2 DesiredSize = CoreWindow->GetDesiredSize();
		CachedGeometry = Geometry::MakeRoot(DesiredSize, SlateLayoutTransform(Vector2::ZeroVector()), SlateRenderTransform(Vector2::ZeroVector()));
	}

	CoreWindow->SendKeyboardEvent(*CachedGeometry, InKey, EKeyboardEvent::Released);
}

void SSlateApplication::OnCursorMoved(Vector2N Location)
{
	std::optional<Geometry> CachedGeometry = CoreWindow->GetCachedGeometry();
	if (!CachedGeometry.has_value())
	{
		Vector2 DesiredSize = CoreWindow->GetDesiredSize();
		CachedGeometry = Geometry::MakeRoot(DesiredSize, SlateLayoutTransform(Vector2::ZeroVector()), SlateRenderTransform(Vector2::ZeroVector()));
	}
}

void SSlateApplication::OnMouseButtonPressed(Vector2N Location, EMouseButton Button)
{
}

void SSlateApplication::OnMouseButtonReleased(Vector2N Location, EMouseButton Button)
{
}

void SSlateApplication::OnMouseWheelScrolled(int32 ScrollDelta)
{
}