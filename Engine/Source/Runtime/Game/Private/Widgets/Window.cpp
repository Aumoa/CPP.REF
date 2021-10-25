// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/Window.h"
#include "Widgets/Viewport.h"
#include "IFrameworkView.h"
#include "Layout/ArrangedChildrens.h"

DEFINE_LOG_CATEGORY(LogSlateWindow);

SWindow::SWindow() : Super()
{
}

void SWindow::InitViewport(IFrameworkView* InFrameworkView)
{
	FrameworkView = InFrameworkView;
	GameViewport = SNew(SViewport)
		.RenderSize(InFrameworkView->GetFrameworkSize())
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
	return Vector2::GetZero();
}

void SWindow::OnArrangeChildren(SArrangedChildrens* ArrangedChildrens, const Geometry& AllottedGeometry)
{
	ESlateVisibility VpVisibility = GameViewport->GetVisibility();
	if (ArrangedChildrens->Accepts(VpVisibility))
	{
		ArrangedChildrens->AddWidget(VpVisibility, AllottedGeometry.MakeChild(
			GameViewport,
			Vector2::GetZero(),
			AllottedGeometry.GetLocalSize()
		));
	}
}