// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GUISkin.gen.h"

namespace Ayla
{
	class GUIStyle;

	ACLASS()
	class RENDERCORE_API GUISkin : public Object
	{
		GENERATED_BODY()

	private:
		APROPERTY()
		PPtr<GUIStyle> m_Label;

	public:
		void SetLabel(RPtr<GUIStyle> label);
		RPtr<GUIStyle> GetLabel();
	};
}