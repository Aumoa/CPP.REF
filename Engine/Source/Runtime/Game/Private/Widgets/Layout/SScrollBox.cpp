// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/Layout/SScrollBox.h"
#include "SScrollBox.gen.cpp"

SScrollBox::SScrollBox() : Super()
{
}

void SScrollBox::Tick(const Geometry& AllottedGeometry, const TimeSpan& deltaTime)
{
	Super::Tick(AllottedGeometry, deltaTime);

	float DesiredSize = GetDesiredSize().Y;
	float Minimum = 0.0f;
	float Maximum = DesiredSize - AllottedGeometry.GetLocalSize().Y;
	if (Minimum > Maximum)
	{
		std::swap(Minimum, Maximum);
	}

	ScrollAmount = std::clamp(ScrollAmount, Minimum, Maximum);
}

bool SScrollBox::OnReceiveMouseWheelScrolled(const Geometry& AllottedGeometry, int32 ScrollDelta, const MouseState& State)
{
	float DesiredSize = GetDesiredSize().Y;

	float Minimum = 0.0f;
	float Maximum = DesiredSize - AllottedGeometry.GetLocalSize().Y;
	if (Minimum > Maximum)
	{
		std::swap(Minimum, Maximum);
	}

	ScrollAmount -= (float)ScrollDelta / 100.0f;
	ScrollAmount = std::clamp(ScrollAmount, Minimum, Maximum);
	return false;
}

void SScrollBox::ScrollToBottom()
{
	ScrollAmount = std::numeric_limits<float>::max();
}

void SScrollBox::OnArrangeChildren(ArrangedChildrens& ArrangedChildrens, const Geometry& AllottedGeometry)
{
	Super::OnArrangeChildren(ArrangedChildrens, AllottedGeometry.MakeChild(Vector2(0.0f, -ScrollAmount), AllottedGeometry.GetLocalSize() + Vector2(0, ScrollAmount)));
}

DEFINE_SLATE_CONSTRUCTOR(SScrollBox, Attr)
{
	INVOKE_SLATE_CONSTRUCTOR_SUPER(Attr);
	SetOrientation(Attr._Orientation);
	SetClipping(EWidgetClipping::ClipToBounds);
}