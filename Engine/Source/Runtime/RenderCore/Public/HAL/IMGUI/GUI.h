// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Rect.h"
#include "Numerics/VectorInterface/Color.h"
#include "GUI.gen.h"

namespace Ayla
{
	class GUISkin;
	class Graphics;

	ACLASS()
	class RENDERCORE_API GUI : public Object
	{
		GENERATED_BODY()

	public:
		virtual Color GetColor() = 0;
		virtual void SetColor(const Color& value) = 0;

		virtual Color GetBackgroundColor() = 0;
		virtual void SetBackgroundColor(const Color& value) = 0;

		virtual Color GetForegroundColor() = 0;
		virtual void SetForegroundColor(const Color& value) = 0;

		virtual RPtr<GUISkin> GetSkin() = 0;
		virtual void SetSkin(RPtr<GUISkin> value) = 0;

		virtual void Label(const RectF& position, String text) = 0;
	};
}