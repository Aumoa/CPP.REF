// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "CoreMinimal.h"
#include "HAL/IMGUI/GUI.h"

namespace Ayla
{
	thread_local RPtr<GUI> s_Current;

	Color GUI::GetColor()
	{
		return s_Current->DoGetColor();
	}

	void GUI::SetColor(const Color& value)
	{
		s_Current->DoSetColor(value);
	}

	Color GUI::GetBackgroundColor()
	{
		return s_Current->DoGetBackgroundColor();
	}

	void GUI::SetBackgroundColor(const Color& value)
	{
		s_Current->DoSetBackgroundColor(value);
	}

	Color GUI::GetForegroundColor()
	{
		return s_Current->DoGetForegroundColor();
	}

	void GUI::SetForegroundColor(const Color& value)
	{
		s_Current->DoSetForegroundColor(value);
	}

	RPtr<GUISkin> GUI::GetSkin()
	{
		return s_Current->DoGetSkin();
	}

	void GUI::SetSkin(RPtr<GUISkin> value)
	{
		s_Current->DoSetSkin(value);
	}

	void GUI::Label(const RectF& position, String text)
	{
		s_Current->DoLabel(position, text);
	}

	void GUI::SetCurrent(RPtr<GUI> instance)
	{
		s_Current = instance;
	}
}