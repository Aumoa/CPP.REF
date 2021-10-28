// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/Viewport.h"

SViewport::SViewport() : Super()
{
}

void SViewport::OnArrangeChildren(ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry)
{
}

DEFINE_SLATE_CONSTRUCTOR(SViewport, Attr)
{
	INVOKE_SLATE_CONSTRUCTOR_SUPER(Attr);
	App = Attr._Application;
}