// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "HAL/IMGUI/GUIStyle.h"
#include "D3D12/D3D12Common.h"
#include "D2D1GUIStyle.gen.h"

namespace Ayla
{
	class GUIContent;

	ACLASS()
	class D2D1GUIStyle : public GUIStyle
	{
		GENERATED_BODY()

	private:
		String m_FontFamilyName = TEXT("Arial");
		float m_FontSize = 14.0f;

	public:
		APROPERTY()
		PPtr<Graphics> m_Graphics;
		ComPtr<IDWriteTextFormat3> m_TextFormat;

	public:
		virtual String GetFontFamilyName() override;
		virtual void SetFontFamilyName(String value) override;

		virtual float GetFontSize() override;
		virtual void SetFontSize(float value) override;

		virtual Vector2F CalcSize(RPtr<GUIContent> content) override;

	public:
		void ApplyModifiedProperties();
	};
}