// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "HAL/IMGUI/GUISkin.h"
#include "HAL/IMGUI/GUIStyle.h"

namespace Ayla
{
	void GUISkin::SetLabel(RPtr<GUIStyle> label)
	{
		m_Label = label;
	}

	RPtr<GUIStyle> GUISkin::GetLabel()
	{
		return m_Label;
	}
}