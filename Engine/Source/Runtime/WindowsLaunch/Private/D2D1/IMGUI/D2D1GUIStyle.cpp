// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "D2D1/IMGUI/D2D1GUIStyle.h"
#include "D3D12/D3D12Graphics.h"

namespace Ayla
{
	String D2D1GUIStyle::GetFontFamilyName()
	{
		return m_FontFamilyName;
	}

	void D2D1GUIStyle::SetFontFamilyName(String value)
	{
		if (m_FontFamilyName != value)
		{
			m_FontFamilyName = value;
			m_TextFormat.Reset();
		}
	}

	float D2D1GUIStyle::GetFontSize()
	{
		return m_FontSize;
	}

	void D2D1GUIStyle::SetFontSize(float value)
	{
		if (m_FontSize != value)
		{
			m_FontSize = value;
			m_TextFormat.Reset();
		}
	}

	void D2D1GUIStyle::ApplyModifiedProperties(RPtr<Graphics> graphics)
	{
		if (m_TextFormat == nullptr)
		{
			auto d3d = (D3D12Graphics*)graphics.Get();
			HR(d3d->m_DWrite->CreateTextFormat(m_FontFamilyName.c_str(), NULL, NULL, 0, m_FontSize, L"en-US", &m_TextFormat));
		}
	}
}