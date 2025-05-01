// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "HAL/IMGUI/GUI.h"

namespace Ayla
{
	struct GUIColorScope
	{
	private:
		Color m_PreviousColor;

	public:
		GUIColorScope(const Color& newColor)
		{
			m_PreviousColor = GUI::GetColor();
			GUI::SetColor(newColor);
		}

		~GUIColorScope() noexcept
		{
			GUI::SetColor(m_PreviousColor);
		}
	};
}