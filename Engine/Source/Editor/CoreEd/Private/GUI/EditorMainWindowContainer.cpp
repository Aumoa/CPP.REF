// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "GUI/EditorMainWindowContainer.h"
#include "GUI/Menu/MenuItem.h"
#include "HAL/IMGUI/GUI.h"
#include "HAL/IMGUI/GUISkin.h"
#include "HAL/IMGUI/GUIStyle.h"
#include "HAL/IMGUI/GUIContent.h"
#include "HAL/IMGUI/GUIColorScope.h"
#include "HAL/IMGUI/GUIEvent.h"

namespace Ayla
{
	void EditorMainWindowContainer::OnGUI(const RectF& position)
	{
		auto position_ = position;

		// menu layout
		auto menuLayout = Rect<>::FillTop(position_, 18.0f);
		position_ = Rect<>::MarginTop(position_, 18.0f);
		DrawMenu(menuLayout);

		// basic toolbar
		auto basicToolbarLayout = Rect<>::FillTop(position_, 22.0f);
		position_ = Rect<>::MarginTop(position_, 22.0f);
		GUI::Label(basicToolbarLayout, TEXT("Basic Toolbar Layout"));

		Super::OnGUI(position_);
	}

	void EditorMainWindowContainer::DrawMenu(const RectF& position)
	{
		{
			GUIColorScope s{ NamedColors::White };
			GUI::FillRect(position);
		}

		if (m_MenuFile == nullptr)
		{
			m_MenuFile = New<MenuItem>();
			m_MenuFile->SetText(TEXT("File"));
		}

		if (m_MenuEdit == nullptr)
		{
			m_MenuEdit = New<MenuItem>();
			m_MenuEdit->SetText(TEXT("Edit"));
		}

		auto position_ = position;
		const float Margin = 8;
		auto e = GUI::GetEvent();

		auto DrawMenuButton = [this, &e, &Margin](const RectF& position, RPtr<GUIContent> content, bool& hover)
		{
			if (e->Type == GUIEvent::Types::MouseMove)
			{
				bool newHover = position.Contains(e->MousePosition);
				if (hover != newHover)
				{
					hover = newHover;
					Repaint();
				}

				if (newHover)
				{
					e->Use();
				}
			}
			else if (e->Type == GUIEvent::Types::Repaint)
			{
				if (hover)
				{
					GUIColorScope s1{ NamedColors::AliceBlue };
					GUI::FillRect(position);
					GUIColorScope s2{ NamedColors::MediumAquamarine };
					GUI::DrawRect(position);
				}

				GUI::Label(Rect<>::Margin(position, Margin, 0, Margin, 0), content);
			}
		};

		auto item1 = m_MenuFile->GetCachedItemSize();
		m_MenuFile->OnGUI(Rect<>::FillLeft(position_, item1.X));
		position_ = Rect<>::MarginLeft(position_, item1.X);

		auto item2 = m_MenuEdit->GetCachedItemSize();
		m_MenuEdit->OnGUI(Rect<>::FillLeft(position_, item2.X));
		position_ = Rect<>::MarginLeft(position_, item2.X);
	}
}