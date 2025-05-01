// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "HAL/IMGUI/GUIContent.h"
#include "D3D12/D3D12Common.h"
#include "D2D1GUIContent.gen.h"

namespace Ayla
{
	ACLASS()
	class D2D1GUIContent : public GUIContent
	{
		GENERATED_BODY()

	private:
		String m_Text;

	public:
		ComPtr<IDWriteTextFormat> m_TextFormat;
		ComPtr<IDWriteTextLayout> m_TextLayout;

	public:
		virtual void SetText(String text) override;
		virtual String GetText() override;

		void ApplyModifiedProperties(IDWriteFactory* factory, const ComPtr<IDWriteTextFormat>& format);
	};
}