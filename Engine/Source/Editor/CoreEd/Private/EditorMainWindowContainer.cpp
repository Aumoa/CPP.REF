// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "EditorMainWindowContainer.h"
#include "HAL/IMGUI/GUI.h"

namespace Ayla
{
	void EditorMainWindowContainer::OnGUI(const RectF& position, RPtr<GUI> gui)
	{
		auto position_ = position;

		// menu layout
		auto menuLayout = Rect<>::FillTop(position_, 18.0f);
		position_ = Rect<>::MarginTop(position_, 18.0f);
		gui->Label(menuLayout, TEXT("Menu Layout"));

		// basic toolbar
		auto basicToolbarLayout = Rect<>::FillTop(position_, 22.0f);
		position_ = Rect<>::MarginTop(position_, 22.0f);
		gui->Label(basicToolbarLayout, TEXT("Basic Toolbar Layout"));

		Super::OnGUI(position_, gui);
	}
}