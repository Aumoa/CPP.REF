// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "D2D1/IMGUI/D2D1GUI.h"
#include "D2D1/IMGUI/D2D1GUIStyle.h"
#include "D2D1/IMGUI/D2D1GUIContent.h"
#include "D3D12/D3D12Graphics.h"
#include "HAL/IMGUI/GUISkin.h"

namespace Ayla
{
	void D2D1GUI::BeginRender()
	{
		SetCurrent(rthis);

		if (m_Skin == nullptr)
		{
			m_Skin = New<GUISkin>();
			m_Skin->SetLabel(CreateStyle());
		}
	}

	void D2D1GUI::EndRender(ID2D1DeviceContext* deviceContext, RPtr<Graphics> graphics)
	{
		for (auto& style : m_Styles)
		{
			((D2D1GUIStyle*)style.Get())->ApplyModifiedProperties();
		}

		for (auto& command : m_RenderCommands)
		{
			command(deviceContext);
		}

		m_RenderCommands.clear();
		m_Styles.clear();

		SetCurrent({});
	}

	Color D2D1GUI::DoGetColor()
	{
		return m_Color;
	}

	void D2D1GUI::DoSetColor(const Color& value)
	{
		m_Color = value;
		m_NeedUpdateBackgroundColor = true;
		m_NeedUpdateForegroundColor = true;
	}

	Color D2D1GUI::DoGetBackgroundColor()
	{
		return m_BackgroundColor;
	}

	void D2D1GUI::DoSetBackgroundColor(const Color& value)
	{
		m_BackgroundColor = value;
		m_NeedUpdateBackgroundColor = true;
	}

	Color D2D1GUI::DoGetForegroundColor()
	{
		return m_ForegroundColor;
	}

	void D2D1GUI::DoSetForegroundColor(const Color& value)
	{
		m_ForegroundColor = value;
		m_NeedUpdateForegroundColor = true;
	}

	RPtr<GUISkin> D2D1GUI::DoGetSkin()
	{
		return m_Skin;
	}

	void D2D1GUI::DoSetSkin(RPtr<GUISkin> value)
	{
		m_Skin = value;
	}

	RPtr<GUIStyle> D2D1GUI::DoCreateStyle()
	{
		auto item = New<D2D1GUIStyle>();
		item->m_Graphics = m_Graphics;
		return item;
	}

	RPtr<GUIContent> D2D1GUI::DoCreateContent(String text)
	{
		auto item = New<D2D1GUIContent>();
		item->SetText(text);
		return item;
	}

#define ASRECT(x) reinterpret_cast<const D2D1_RECT_F&>(x)
#define ASCOLOR(x) reinterpret_cast<const D2D1_COLOR_F&>(x)

	void D2D1GUI::DoLabel(const RectF& position, String text)
	{
		auto style = m_Skin->GetLabel();
		m_Styles.emplace_back(style);

		m_RenderCommands.emplace_back([
			this,
			position = position,
			text,
			style,
			needUpdateForegroundColor = m_NeedUpdateForegroundColor,
			color = m_Color,
			foregroundColor = m_ForegroundColor
		](ID2D1DeviceContext* deviceContext)
		{
			if (needUpdateForegroundColor)
			{
				UpdateColor(deviceContext, m_ForegroundColorBrush, Color::AlphaBlend(foregroundColor, color));
			}

			auto* d2dstyle = (D2D1GUIStyle*)style.Get();
			deviceContext->DrawTextW(text.c_str(), (UINT32)text.length(), d2dstyle->m_TextFormat.Get(), ASRECT(position), m_ForegroundColorBrush.Get());
		});

		m_NeedUpdateForegroundColor = false;
	}

	void D2D1GUI::DoLabel(const RectF& position, RPtr<GUIContent> content)
	{
		auto style = m_Skin->GetLabel();
		m_Styles.emplace_back(style);

		m_RenderCommands.emplace_back([
			this,
			position = position,
			content,
			style,
			needUpdateForegroundColor = m_NeedUpdateForegroundColor,
			color = m_Color,
			foregroundColor = m_ForegroundColor
		](ID2D1DeviceContext* deviceContext)
		{
			if (needUpdateForegroundColor)
			{
				UpdateColor(deviceContext, m_ForegroundColorBrush, Color::AlphaBlend(foregroundColor, color));
			}

			auto* d2dstyle = (D2D1GUIStyle*)style.Get();
			auto* c = (D2D1GUIContent*)content.Get();
			auto* g = (D3D12Graphics*)m_Graphics.Get();
			c->ApplyModifiedProperties(g->m_DWrite.Get(), d2dstyle->m_TextFormat.Get());
			c->m_TextLayout->SetMaxWidth(Rect<>::Width(position));
			c->m_TextLayout->SetMaxHeight(Rect<>::Height(position));
			deviceContext->DrawTextLayout(D2D1::Point2F(position.Left, position.Top), c->m_TextLayout.Get(), m_ForegroundColorBrush.Get());
		});

		m_NeedUpdateForegroundColor = false;
	}

	void D2D1GUI::DoDrawRect(const RectF& position, float strokeWidth)
	{
		m_RenderCommands.emplace_back([
			this,
			position = position,
			strokeWidth,
			needUpdateForegroundColor = m_NeedUpdateForegroundColor,
			color = m_Color,
			foregroundColor = m_ForegroundColor
		](ID2D1DeviceContext* deviceContext)
		{
			if (needUpdateForegroundColor)
			{
				UpdateColor(deviceContext, m_ForegroundColorBrush, Color::AlphaBlend(foregroundColor, color));
			}

			deviceContext->DrawRectangle(ASRECT(position), m_ForegroundColorBrush.Get(), strokeWidth, nullptr);
		});

		m_NeedUpdateBackgroundColor = false;
	}

	void D2D1GUI::DoFillRect(const RectF& position)
	{
		m_RenderCommands.emplace_back([
			this,
			position = position,
			needUpdateForegroundColor = m_NeedUpdateForegroundColor,
			color = m_Color,
			foregroundColor = m_ForegroundColor
		](ID2D1DeviceContext* deviceContext)
		{
			if (needUpdateForegroundColor)
			{
				UpdateColor(deviceContext, m_ForegroundColorBrush, Color::AlphaBlend(foregroundColor, color));
			}

			deviceContext->FillRectangle(ASRECT(position), m_ForegroundColorBrush.Get());
		});

		m_NeedUpdateBackgroundColor = false;
	}

	void D2D1GUI::UpdateColor(ID2D1DeviceContext* deviceContext, ComPtr<ID2D1SolidColorBrush>& brush, const Color& color)
	{
		if (brush == nullptr)
		{
			HR(deviceContext->CreateSolidColorBrush(ASCOLOR(color), &brush));
		}
		else
		{
			brush->SetColor(ASCOLOR(color));
		}
	}

#undef ASRECT
#undef ASCOLOR
}