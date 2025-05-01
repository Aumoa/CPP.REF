// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GUI/DockableWindowContainer.h"
#include "EditorMainWindowContainer.gen.h"

namespace Ayla
{
	class GUIContent;

	ACLASS()
	class EditorMainWindowContainer : public DockableWindowContainer
	{
		GENERATED_BODY()

	private:
		APROPERTY()
		PPtr<GUIContent> m_MenuFileContent;
		Vector2F m_MenuFileContentSize;
		APROPERTY()
		PPtr<GUIContent> m_MenuEditContent;
		Vector2F m_MenuEditContentSize;

	protected:
		virtual void OnGUI(const RectF& position) override;

	private:
		void DrawMenu(const RectF& position);
	};
}