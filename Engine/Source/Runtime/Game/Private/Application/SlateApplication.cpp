// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Application/SlateApplication.h"
#include "Application/Window.h"
#include "Draw/PaintArgs.h"
#include "Draw/SlateRenderer.h"
#include "Draw/SlateDrawCollector.h"
#include "Draw/IRenderSlateElement.h"
#include "SceneRendering/SceneRenderContext.h"
#include "EngineSubsystems/GameRenderSystem.h"
#include "RHI/RHIInterfaces.h"
#include "GameFramework/LocalPlayer.h"
#include "Input/IPlatformKeyboard.h"
#include "Input/IPlatformMouse.h"
#include "Input/IPlatformIME.h"
#include "Numerics/VectorInterface/Vector.h"
#include "RenderThread.h"
#include "IApplicationInterface.h"

GENERATE_BODY(SSlateApplication);

DECLARE_STAT_GROUP("Slate", STATGROUP_Slate);
DECLARE_CYCLE_STAT("TickAndPaint", STAT_TickAndPaint, STATGROUP_Slate);
DECLARE_CYCLE_STAT("  Prepass", STAT_Prepass, STATGROUP_Slate);
DECLARE_CYCLE_STAT("  Tick", STAT_Tick, STATGROUP_Slate);
DECLARE_CYCLE_STAT("  Paint", STAT_Paint, STATGROUP_Slate);
DECLARE_CYCLE_STAT("  SortAndFlush", STAT_SortAndFlush, STATGROUP_Slate);

SSlateApplication::SSlateApplication() : Super()
{
}

SSlateApplication::~SSlateApplication()
{
	// Remove all delegates for dispose DLL link.
	auto& App = IApplicationInterface::Get();

	//auto& PlatformKeyboard = App.GetPlatformKeyboard();
	//PlatformKeyboard.KeyPressed.RemoveAll(this);
	//PlatformKeyboard.KeyReleased.RemoveAll(this);

	//auto& PlatformMouse = App.GetPlatformMouse();
	//PlatformMouse.CursorMoved.RemoveAll(this);
	//PlatformMouse.MouseButtonPressed.RemoveAll(this);
	//PlatformMouse.MouseButtonReleased.RemoveAll(this);
	//PlatformMouse.MouseWheelScrolled.RemoveAll(this);
}

void SSlateApplication::Init(IApplicationInterface* InApplication)
{
	LocalPlayer = gcnew SLocalPlayer(this);
	DrawCollector = gcnew SSlateDrawCollector();

	CoreWindow = CreateCoreWindow();
	CoreWindow->InitViewport();

	//auto& PlatformKeyboard = InApplication->GetPlatformKeyboard();
	//PlatformKeyboard.KeyPressed.AddSObject(this, &SSlateApplication::OnPlatformKeyPressed);
	//PlatformKeyboard.KeyReleased.AddSObject(this, &SSlateApplication::OnPlatformKeyReleased);

	//auto& PlatformMouse = InApplication->GetPlatformMouse();
	//PlatformMouse.CursorMoved.AddSObject(this, &SSlateApplication::OnCursorMoved);
	//PlatformMouse.MouseButtonPressed.AddSObject(this, &SSlateApplication::OnMouseButtonPressed);
	//PlatformMouse.MouseButtonReleased.AddSObject(this, &SSlateApplication::OnMouseButtonReleased);
	//PlatformMouse.MouseWheelScrolled.AddSObject(this, &SSlateApplication::OnMouseWheelScrolled);

	auto& PlatformIME = InApplication->GetPlatformIME();
	PlatformIME.IME.AddObject(this, &SSlateApplication::OnIME);
}

void SSlateApplication::TickAndPaint(float InDeltaTime)
{
	SCOPE_CYCLE_COUNTER(STAT_TickAndPaint);

	if (Device == nullptr)
	{
		Device = GEngine->GetEngineSubsystem<SGameRenderSystem>()->GetRHIDevice();
	}

	// Calculate prepass layout.
	Vector2 DesiredSize;
	std::optional<Geometry> AllottedGeometry;

	{
		SCOPE_CYCLE_COUNTER(STAT_Prepass);
		CoreWindow->PrepassLayout();
		DesiredSize = Cast<float>(IApplicationInterface::Get().GetViewportSize());
		AllottedGeometry = Geometry::MakeRoot(DesiredSize, SlateLayoutTransform(Vector2::Zero()), SlateRenderTransform(Translate2D::Identity()));
	}

	{
		SCOPE_CYCLE_COUNTER(STAT_Tick);
		CoreWindow->Tick(*AllottedGeometry, InDeltaTime);
	}

	{
		SCOPE_CYCLE_COUNTER(STAT_Paint);
		Rect CullingRect = Rect(0.0f, 0.0f, DesiredSize.X, DesiredSize.Y);
		CoreWindow->Paint(PaintArgs::InitPaintArgs(Device, InDeltaTime), *AllottedGeometry, CullingRect, DrawCollector, 0, true);
	}

	{
		std::vector<SSlateDrawCollector::RenderElement> CachedElements;
		SCOPE_CYCLE_COUNTER(STAT_SortAndFlush);
		DrawCollector->SortByLayer();
		DrawCollector->FlushElements(CachedElements);

		CacheRenderElements_GameThread(std::move(CachedElements));
	}
}

void SSlateApplication::DrawElements(IRHIDeviceContext2D* CommandList, SSlateRenderer* Renderer)
{
	for (auto& Element : RenderElements)
	{
		if (Element.Element)
		{
			Element.Element->ToString();
		}
	}
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

void SSlateApplication::OnIME(IMEEvent EventArgs)
{
	CoreWindow->SendIMEEvent(MakeRoot(), EventArgs);
}

void SSlateApplication::CacheRenderElements_GameThread(std::vector<SSlateDrawCollector::RenderElement> Elements)
{
	RenderThread::Get()->EnqueueRenderThreadWork(this, [this, Elements = std::move(Elements)](auto) mutable
	{
		RenderElements = std::move(Elements);
	});
}

Geometry SSlateApplication::MakeRoot()
{
	Vector2 DesiredSize = Cast<float>(IApplicationInterface::Get().GetViewportSize());
	Geometry AllottedGeometry = Geometry::MakeRoot(DesiredSize, SlateLayoutTransform(Vector2::Zero()), SlateRenderTransform(Translate2D::Identity()));
	return AllottedGeometry;
}