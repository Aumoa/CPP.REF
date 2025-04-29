// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12/D3D12Common.h"
#include "HAL/IMGUI/GUI.h"
#include "Numerics/VectorInterface/Color.h"
#include "D2D1GUI.gen.h"

namespace Ayla
{
	class GUIStyle;

	ACLASS()
	class D2D1GUI : public GUI
	{
		GENERATED_BODY()

	private:
		APROPERTY()
		PPtr<GUISkin> m_Skin;
		std::vector<Action<ID2D1DeviceContext*>> m_RenderCommands;
		std::vector<RPtr<GUIStyle>> m_Styles;

		Color m_Color = NamedColors::White;
		Color m_BackgroundColor = NamedColors::Transparent;
		bool m_NeedUpdateBackgroundColor = true;
		ComPtr<ID2D1SolidColorBrush> m_BackgroundColorBrush;
		Color m_ForegroundColor = NamedColors::White;
		bool m_NeedUpdateForegroundColor = true;
		ComPtr<ID2D1SolidColorBrush> m_ForegroundColorBrush;

	public:
		void BeginRender();
		void EndRender(ID2D1DeviceContext* deviceContext, RPtr<Graphics> graphics);

		virtual Color GetColor() override;
		virtual void SetColor(const Color& value) override;

		virtual Color GetBackgroundColor() override;
		virtual void SetBackgroundColor(const Color& value) override;

		virtual Color GetForegroundColor() override;
		virtual void SetForegroundColor(const Color& value) override;

		virtual RPtr<GUISkin> GetSkin() override;
		virtual void SetSkin(RPtr<GUISkin> value) override;

		virtual void Label(const RectF& position, String text) override;

	private:
		void UpdateColor(ID2D1DeviceContext* deviceContext, ComPtr<ID2D1SolidColorBrush>& brush, const Color& color);
	};
}