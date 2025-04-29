// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GUIStyle.gen.h"

namespace Ayla
{
	class Graphics;

	ACLASS()
	class RENDERCORE_API GUIStyle : public Object
	{
		GENERATED_BODY()

	public:
		virtual String GetFontFamilyName() = 0;
		virtual void SetFontFamilyName(String value) = 0;

		virtual float GetFontSize() = 0;
		virtual void SetFontSize(float value) = 0;

		virtual void ApplyModifiedProperties(RPtr<Graphics> graphics) = 0;
	};
}