// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "GUI/DockableWindowContainer.h"
#include "HAL/IMGUI/GUI.h"

namespace Ayla
{
	void DockableWindowContainer::OnGUI(const RectF& position)
	{
		RectF position_ = position;

		// tab layout
		auto tabLayout = Rect<>::FillTop(position_, 18.0f);
		position_ = Rect<>::MarginTop(position_, 18.0f);
		GUI::Label(tabLayout, TEXT("Tab Layout"));

		Super::OnGUI(position_);
	}
}