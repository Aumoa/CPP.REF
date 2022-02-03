// Copyright 2020-2022 Aumoa.lib. All right reserved.

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
		.RenderSize(IApplicationInterface::Get().GetViewportSize());

	InvalidateLayoutAndVolatility();
}

SViewport* SWindow::GetGameViewport()
{
	return GameViewport;
}

Vector2 SWindow::ComputeDesiredSize()
{
	if (GameViewport)
	{
		return GameViewport->GetDesiredSize();
	}
	return Vector2::Zero();
}

void SWindow::OnArrangeChildren(ArrangedChildrens& ArrangedChildrens, const Geometry& AllottedGeometry)
{
	ESlateVisibility VpVisibility = GameViewport->GetVisibility();
	if (ArrangedChildrens.Accepts(VpVisibility))
	{
		ArrangedChildrens.AddWidget(VpVisibility, AllottedGeometry.MakeChild(
			GameViewport,
			Vector2::Zero(),
			AllottedGeometry.GetLocalSize()
		));
	}
}

size_t SWindow::NumChildrens()
{
	return GameViewport ? 1 : 0;
}

SWidget* SWindow::GetChildrenAt(size_t IndexOf)
{
	return GameViewport;
}