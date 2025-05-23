// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "CoreMinimal.h"
#include "HAL/IMGUI/GUI.h"
#include "HAL/IMGUI/GUIEvent.h"

namespace Ayla
{
	thread_local RPtr<GUI> s_Current;
	thread_local const GUIEvent* s_Event;

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

	const GUIEvent* GUI::GetEvent()
	{
		return s_Event;
	}

	void GUI::SetEvent(const GUIEvent* event)
	{
		s_Event = event;
	}

	RPtr<GUIStyle> GUI::CreateStyle()
	{
		return s_Current->DoCreateStyle();
	}

	RPtr<GUIContent> GUI::CreateContent(String text)
	{
		return s_Current->DoCreateContent(text);
	}

	void GUI::Label(const RectF& position, String text)
	{
		if (s_Event->Type == GUIEvent::Types::Repaint)
		{
			s_Current->DoLabel(position, text);
		}
	}

	void GUI::Label(const RectF& position, RPtr<GUIContent> content)
	{
		if (s_Event->Type == GUIEvent::Types::Repaint)
		{
			s_Current->DoLabel(position, content);
		}
	}

	void GUI::DrawRect(const RectF& position, float strokeWidth)
	{
		if (s_Event->Type == GUIEvent::Types::Repaint)
		{
			s_Current->DoDrawRect(position, strokeWidth);
		}
	}

	void GUI::FillRect(const RectF& position)
	{
		if (s_Event->Type == GUIEvent::Types::Repaint)
		{
			s_Current->DoFillRect(position);
		}
	}

	void GUI::SetCurrent(RPtr<GUI> instance)
	{
		s_Current = instance;
	}
}