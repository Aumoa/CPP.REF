// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Vector.h"
#include "Numerics/VectorInterface/Rect.h"
#include "MenuItem.gen.h"

namespace Ayla
{
	class GUIContent;

	ACLASS()
	class COREED_API MenuItem : public Object
	{
		GENERATED_BODY()

	private:
		static constexpr float TextMargin = 8.0f;

	private:
		APROPERTY()
		PPtr<GUIContent> m_Content;
		bool m_SelfCreated = false;
		bool m_Hover = false;
		Vector2F m_ContentSize;

	public:
		void SetContent(RPtr<GUIContent> content);
		void SetText(String text);
		Vector2F GetCachedContentSize();
		Vector2F GetCachedItemSize();

	public:
		void OnGUI(const RectF& position);

	private:
		void CacheContentSize();
	};
}