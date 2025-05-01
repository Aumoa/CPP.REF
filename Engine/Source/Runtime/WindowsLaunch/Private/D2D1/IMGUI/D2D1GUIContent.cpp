// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "D2D1GUIContent.h"

namespace Ayla
{
	void D2D1GUIContent::SetText(String text)
	{
		if (m_Text != text)
		{
			m_Text = text;
			m_TextFormat.Reset();
			m_TextLayout.Reset();
		}
	}

	String D2D1GUIContent::GetText()
	{
		return m_Text;
	}

	void D2D1GUIContent::ApplyModifiedProperties(IDWriteFactory* factory, const ComPtr<IDWriteTextFormat>& format)
	{
		if (m_TextFormat == nullptr || m_TextFormat != format)
		{
			m_TextFormat = format;
			HR(factory->CreateTextLayout(m_Text.c_str(), (UINT32)m_Text.length(), format.Get(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), &m_TextLayout));
			m_TextFormat = format;
		}
	}
}