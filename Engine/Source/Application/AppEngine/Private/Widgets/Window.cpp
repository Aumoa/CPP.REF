// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/Window.h"
#include "Draw/PaintArgs.h"
#include "Draw/SlateWindowElementList.h"
#include "RHI/IRHIDevice.h"
#include "RHI/IRHIBrush.h"
#include "PlatformCore/IApplicationInterface.h"

SWindow::SWindow() : Super()
{
}

Vector2 SWindow::GetDesiredSize()
{
	if (App)
	{
		return App->GetViewportSize().Cast<float>();
	}
	return Vector2::ZeroVector();
}

void SWindow::TickAndPaint(float InDeltaTime, IRHIDevice* Device)
{
	Vector2 DesiredSize = GetDesiredSize();
	Rect CullingRect = Rect(0, 0, DesiredSize.X, DesiredSize.Y);
	Geometry AllottedGeometry = Geometry::MakeRoot(DesiredSize, SlateLayoutTransform::Identity(), SlateRenderTransform::Identity());

	Tick(AllottedGeometry, InDeltaTime);

	auto Elements = SlateWindowElementList(this);
	Paint(PaintArgs(this, InDeltaTime, 1.0f), AllottedGeometry, CullingRect, Elements, 0, true);

	Elements.SortByLayer();
	for (auto& Element : Elements.GetElements())
	{
		if (Element.Transform.HasRenderTransform() && Element.Brush)
		{
			SlateRenderTransform RenderTransform = Element.Transform.GetAccumulatedRenderTransform();
			Vector2 LocalSize = Element.Transform.GetLocalSize();

			Device->SetTransform(RenderTransform);
			Device->FillRectangle(Element.Brush, Rect(0, 0, LocalSize.X, LocalSize.Y));
		}
	}
}

DEFINE_SLATE_CONSTRUCTOR(SWindow, Attr)
{
	INVOKE_SLATE_CONSTRUCTOR_SUPER(Attr);
	App = Attr._Application;
}