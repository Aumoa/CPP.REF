// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "GUI/EditorMainWindowContainer.h"
#include "HAL/IMGUI/GUI.h"
#include "HAL/IMGUI/GUISkin.h"
#include "HAL/IMGUI/GUIStyle.h"
#include "HAL/IMGUI/GUIContent.h"
#include "HAL/IMGUI/GUIColorScope.h"

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
		GUI::SetColor(NamedColors::White);
		GUI::FillRect(position);

		if (m_MenuFileContent == nullptr)
		{
			m_MenuFileContent = GUI::CreateContent(TEXT("File"));
			auto label = GUI::GetSkin()->GetLabel();
			m_MenuFileContentSize = label->CalcSize(m_MenuFileContent);
		}

		if (m_MenuEditContent == nullptr)
		{
			m_MenuEditContent = GUI::CreateContent(TEXT("Edit"));
			auto label = GUI::GetSkin()->GetLabel();
			m_MenuEditContentSize = label->CalcSize(m_MenuEditContent);
		}

		auto position_ = position;
		const float Margin = 8;

		auto scope1 = GUIColorScope(NamedColors::Black);
		{
			auto item1 = Rect<>::FillLeft(Rect<>::MarginLeft(position_, Margin), m_MenuFileContentSize.X);
			position_ = Rect<>::MarginLeft(position_, Margin + m_MenuFileContentSize.X + Margin);
			GUI::Label(item1, m_MenuFileContent);

			auto item2 = Rect<>::FillLeft(Rect<>::MarginLeft(position_, Margin), m_MenuEditContentSize.X);
			position_ = Rect<>::MarginLeft(position_, Margin + m_MenuEditContentSize.X + Margin);
			GUI::Label(item2, m_MenuEditContent);
		}
	}
}