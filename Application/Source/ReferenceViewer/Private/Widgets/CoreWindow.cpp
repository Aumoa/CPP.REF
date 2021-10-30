// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/CoreWindow.h"
#include "Widgets/Panel/CanvasPanel.h"
#include "Widgets/Image/Image.h"
#include "Draw/PaintArgs.h"

SCoreWindow::SCoreWindow() : Super()
{
}

void SCoreWindow::Tick(const Geometry& AllottedGeometry, float InDeltaTime)
{
	if (CanvasPanel)
	{
		CanvasPanel->Tick(AllottedGeometry, InDeltaTime);
	}
}

int32 SCoreWindow::OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SlateWindowElementList& InDrawElements, int32 InLayer, bool bParentEnabled)
{
	if (CanvasPanel)
	{
		PaintArgs NewArgs = Args.WithNewParent(this);
		return CanvasPanel->Paint(Args, AllottedGeometry, CullingRect, InDrawElements, InLayer, bParentEnabled);
	}
	return InLayer;
}

void SCoreWindow::OnArrangeChildren(ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry)
{
	if (CanvasPanel)
	{
		CanvasPanel->ArrangeChildren(InoutArrangedChildrens, AllottedGeometry);
	}
}

DEFINE_SLATE_CONSTRUCTOR(SCoreWindow, Attr)
{
	INVOKE_SLATE_CONSTRUCTOR_SUPER(std::move(Attr));
	CanvasPanel = SNew(SCanvasPanel)
		+SCanvasPanel::Slot()
		.Offset(10.0f, 10.0f, 0.0f, 0.0f)
		.bAutoSize(true)
		.ZOrder(2.0f)
		[
			SAssignNew(RedBox, SImage)
			.Brush(nullptr, Vector2(100.0f, 100.0f), NamedColors::Red)
		]
		+SCanvasPanel::Slot()
		.Offset(10.0f, 60.0f, 0.0f, 0.0f)
		.bAutoSize(true)
		.ZOrder(3.0f)
		[
			SAssignNew(GreenBox, SImage)
			.Brush(nullptr, Vector2(100.0f, 100.0f), NamedColors::Green)
		]
		+SCanvasPanel::Slot()
		.Offset(10.0f, 110.0f, 0.0f, 0.0f)
		.bAutoSize(true)
		.ZOrder(1.0f)
		[
			SAssignNew(BlueBox, SImage)
			.Brush(nullptr, Vector2(100.0f, 100.0f), NamedColors::Blue)
		];
}