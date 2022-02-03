// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Application/EditorWindow.h"
#include "Application/Viewport.h"
#include "IApplicationInterface.h"
#include "Widgets/DebugCanvas.h"
#include "Widgets/LogConsole.h"
#include "Widgets/Panel/CanvasPanel.h"
#include "Widgets/Panel/VerticalBoxPanel.h"
#include "Widgets/Panel/HorizontalBoxPanel.h"
#include "Widgets/Image/Image.h"

GENERATE_BODY(SEditorWindow);

SEditorWindow::SEditorWindow() : Super()
{
}

void SEditorWindow::InitViewport()
{
	Super::InitViewport();

	//EditorLayout =
	//	SNew(SHorizontalBoxPanel)
	//	+SHorizontalBoxPanel::SSlot()
	//	.SizeParam(ESizeRule::Stretch, 1.0f)
	//	.VAlignment(EVerticalAlignment::Fill)
	//	.HAlignment(EHorizontalAlignment::Fill)
	//	[
	//		SNew(SVerticalBoxPanel)
	//		// GameViewport
	//		+SVerticalBoxPanel::SSlot()
	//		.SizeParam(ESizeRule::Stretch, 1.0f)
	//		[
	//			EditorViewport = GetGameViewport()
	//		]
	//		// LogConsole
	//		+SVerticalBoxPanel::SSlot()
	//		.SizeParam(ESizeRule::Auto, 1.0f)
	//		.HAlignment(EHorizontalAlignment::Fill)
	//		[
	//			SNew(SLogConsole)
	//		]
	//	]
	//	+SHorizontalBoxPanel::SSlot()
	//	.SizeParam(ESizeRule::Auto, 1.0f)
	//	.VAlignment(EVerticalAlignment::Fill)
	//	[
	//		SNew(SImage)
	//		.TintColor(NamedColors::LightGreen)
	//	]
	//	;

	//EditorViewport->AddToViewport(SNew(SDebugCanvas));
}

SViewport* SEditorWindow::GetEditorViewport()
{
	return EditorViewport;
}

Vector2 SEditorWindow::ComputeDesiredSize()
{
	return EditorLayout->GetDesiredSize();
}

void SEditorWindow::OnArrangeChildren(ArrangedChildrens& ArrangedChildrens, const Geometry& AllottedGeometry)
{
	EditorLayout->ArrangeChildren(ArrangedChildrens, AllottedGeometry);
}

size_t SEditorWindow::NumChildrens()
{
	return EditorLayout->NumChildrens();
}

SWidget* SEditorWindow::GetChildrenAt(size_t IndexOf)
{
	return EditorLayout->GetChildrenAt(IndexOf);
}