// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "D2D1/IMGUI/D2D1GUIStyle.h"
#include "D2D1/IMGUI/D2D1GUIContent.h"
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

	Vector2F D2D1GUIStyle::CalcSize(RPtr<GUIContent> content)
	{
		ApplyModifiedProperties();
		auto c = (D2D1GUIContent*)content.Get();
		auto g = (D3D12Graphics*)m_Graphics.Get();
		c->ApplyModifiedProperties(g->m_DWrite.Get(), m_TextFormat);
		DWRITE_TEXT_METRICS metrics;
		HR(c->m_TextLayout->GetMetrics(&metrics));
		return Vector2F(metrics.width, metrics.height);
	}

	void D2D1GUIStyle::ApplyModifiedProperties()
	{
		if (m_TextFormat == nullptr)
		{
			auto d3d = (D3D12Graphics*)m_Graphics.Get();
			HR(d3d->m_DWrite->CreateTextFormat(m_FontFamilyName.c_str(), NULL, NULL, 0, m_FontSize, L"en-US", &m_TextFormat));
		}
	}
}