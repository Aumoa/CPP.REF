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

	protected:
		virtual Color DoGetColor() override;
		virtual void DoSetColor(const Color& value) override;

		virtual Color DoGetBackgroundColor() override;
		virtual void DoSetBackgroundColor(const Color& value) override;

		virtual Color DoGetForegroundColor() override;
		virtual void DoSetForegroundColor(const Color& value) override;

		virtual RPtr<GUISkin> DoGetSkin() override;
		virtual void DoSetSkin(RPtr<GUISkin> value) override;

		virtual void DoLabel(const RectF& position, String text) override;

	private:
		void UpdateColor(ID2D1DeviceContext* deviceContext, ComPtr<ID2D1SolidColorBrush>& brush, const Color& color);
	};
}