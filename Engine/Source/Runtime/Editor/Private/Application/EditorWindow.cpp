// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Application/EditorWindow.h"
#include "Application/Viewport.h"
#include "IApplicationInterface.h"
#include "Widgets/DebugCanvas.h"

GENERATE_BODY(SEditorWindow);

SEditorWindow::SEditorWindow() : Super()
{
}

void SEditorWindow::InitViewport()
{
	Super::InitViewport();

	EditorViewport = SNew(SViewport)
		.RenderSize(IApplicationInterface::Get().GetViewportSize())
		.RenderTargetFormat(ERHIPixelFormat::B8G8R8A8_UNORM);
	EditorViewport->AddToViewport(SAssignNew(DebugCanvas, SDebugCanvas));
}

SViewport* SEditorWindow::GetEditorViewport()
{
	return EditorViewport;
}

Vector2 SEditorWindow::GetDesiredSize()
{
	if (EditorViewport)
	{
		return EditorViewport->GetDesiredSize();
	}
	return Vector2::ZeroVector();
}

void SEditorWindow::OnArrangeChildren(ArrangedChildrens& ArrangedChildrens, const Geometry& AllottedGeometry)
{
	ESlateVisibility VpVisibility = EditorViewport->GetVisibility();
	if (ArrangedChildrens.Accepts(VpVisibility))
	{
		ArrangedChildrens.AddWidget(VpVisibility, AllottedGeometry.MakeChild(
			EditorViewport,
			Vector2::ZeroVector(),
			AllottedGeometry.GetLocalSize()
		));
	}
}