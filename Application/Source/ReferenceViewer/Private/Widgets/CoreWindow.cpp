// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/CoreWindow.h"
#include "Widgets/Image/Image.h"

SCoreWindow::SCoreWindow() : Super()
{
}

void SCoreWindow::OnArrangeChildren(ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry)
{
	if (InoutArrangedChildrens.Accepts(Image->GetVisibility()))
	{
		InoutArrangedChildrens.AddWidget(Image->GetVisibility(), AllottedGeometry.MakeChild(
			Image,
			Vector2(10.0f, 10.0f),
			Image->GetDesiredSize()
		));
	}
}

DEFINE_SLATE_CONSTRUCTOR(SCoreWindow, Attr)
{
	INVOKE_SLATE_CONSTRUCTOR_SUPER(std::move(Attr));
	Image = SNew(SImage).Brush(nullptr, Vector2(100.0f, 100.0f), NamedColors::Red);
}