// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Rect.h"
#include "Numerics/VectorInterface/Color.h"
#include "GUI.gen.h"

namespace Ayla
{
	class GUISkin;
	class GUIStyle;
	class GUIContent;
	class Graphics;

	ACLASS()
	class RENDERCORE_API GUI : public Object
	{
		GENERATED_BODY()

	public:
		static Color GetColor();
		static void SetColor(const Color& value);

		static Color GetBackgroundColor();
		static void SetBackgroundColor(const Color& value);

		static Color GetForegroundColor();
		static void SetForegroundColor(const Color& value);

		static RPtr<GUISkin> GetSkin();
		static void SetSkin(RPtr<GUISkin> value);

		static RPtr<GUIStyle> CreateStyle();
		static RPtr<GUIContent> CreateContent(String text);

		static void Label(const RectF& position, String text);
		static void Label(const RectF& position, RPtr<GUIContent> content);

		static void DrawRect(const RectF& position, float strokeWidth = 1.0f);

		static void FillRect(const RectF& position);

	protected:
		static void SetCurrent(RPtr<GUI> instance);

		virtual Color DoGetColor() = 0;
		virtual void DoSetColor(const Color& value) = 0;

		virtual Color DoGetBackgroundColor() = 0;
		virtual void DoSetBackgroundColor(const Color& value) = 0;

		virtual Color DoGetForegroundColor() = 0;
		virtual void DoSetForegroundColor(const Color& value) = 0;

		virtual RPtr<GUISkin> DoGetSkin() = 0;
		virtual void DoSetSkin(RPtr<GUISkin> value) = 0;

		virtual RPtr<GUIStyle> DoCreateStyle() = 0;
		virtual RPtr<GUIContent> DoCreateContent(String text) = 0;

		virtual void DoLabel(const RectF& position, String text) = 0;
		virtual void DoLabel(const RectF& position, RPtr<GUIContent> content) = 0;

		virtual void DoDrawRect(const RectF& position, float strokeWidth = 1.0f) = 0;
		
		virtual void DoFillRect(const RectF& position) = 0;
	};
}