// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Application/Window.h"
#include "Application/Viewport.h"
#include "IApplicationInterface.h"

GENERATE_BODY(SWindow);
DEFINE_LOG_CATEGORY(LogSlateWindow);

SWindow::SWindow() : Super()
{
}

void SWindow::InitViewport()
{
	GameViewport = SNew(SViewport)
		.RenderSize(IApplicationInterface::Get().GetViewportSize())
		.RenderTargetFormat(ERHIPixelFormat::B8G8R8A8_UNORM);
}

SViewport* SWindow::GetGameViewport()
{
	return GameViewport;
}

Vector2 SWindow::GetDesiredSize()
{
	if (GameViewport)
	{
		return GameViewport->GetDesiredSize();
	}
	return Vector2::ZeroVector();
}

void SWindow::OnArrangeChildren(ArrangedChildrens& ArrangedChildrens, const Geometry& AllottedGeometry)
{
	ESlateVisibility VpVisibility = GameViewport->GetVisibility();
	if (ArrangedChildrens.Accepts(VpVisibility))
	{
		ArrangedChildrens.AddWidget(VpVisibility, AllottedGeometry.MakeChild(
			GameViewport,
			Vector2::ZeroVector(),
			AllottedGeometry.GetLocalSize()
		));
	}
}