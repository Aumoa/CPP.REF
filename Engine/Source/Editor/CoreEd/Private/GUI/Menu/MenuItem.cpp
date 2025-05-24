// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "GUI/Menu/MenuItem.h"
#include "HAL/IMGUI/GUI.h"
#include "HAL/IMGUI/GUIContent.h"
#include "HAL/IMGUI/GUISkin.h"
#include "HAL/IMGUI/GUIStyle.h"
#include "HAL/IMGUI/GUIEvent.h"
#include "HAL/IMGUI/GUIColorScope.h"

namespace Ayla
{
	void MenuItem::SetContent(RPtr<GUIContent> content)
	{
		m_Content = content;
		m_SelfCreated = false;
		CacheContentSize();
	}

	void MenuItem::SetText(String text)
	{
		if (m_Content == nullptr || m_SelfCreated == false)
		{
			m_Content = GUI::CreateContent(text);
			m_SelfCreated = true;
		}
		else
		{
			m_Content->SetText(text);
		}

		CacheContentSize();
	}

	Vector2F MenuItem::GetCachedContentSize()
	{
		return m_ContentSize;
	}

	Vector2F MenuItem::GetCachedItemSize()
	{
		return Vector2F(m_ContentSize.X + TextMargin + TextMargin, 18.0f);
	}

	void MenuItem::OnGUI(const RectF& position)
	{
		auto e = GUI::GetEvent();

		if (e->Type == GUIEvent::Types::MouseMove)
		{
			m_Hover = position.Contains(e->MousePosition);
		}
		else if (e->Type == GUIEvent::Types::Repaint)
		{
			if (m_Hover)
			{
				GUIColorScope s1{ NamedColors::AliceBlue };
				GUI::FillRect(position);
				GUIColorScope s2{ NamedColors::MediumAquamarine };
				GUI::DrawRect(position);
			}

			GUIColorScope s2{ NamedColors::Black };
			GUI::Label(Rect<>::Margin(position, TextMargin, 0, TextMargin, 0), m_Content);
		}
	}

	void MenuItem::CacheContentSize()
	{
		if (m_Content == nullptr)
		{
			m_ContentSize = Vector2F::Zero();
		}
		else
		{
			m_ContentSize = GUI::GetSkin()->GetLabel()->CalcSize(m_Content);
		}
	}
}