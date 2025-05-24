// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GUI/DockableWindowContainer.h"
#include "EditorMainWindowContainer.gen.h"

namespace Ayla
{
	class GUIContent;
	class MenuItem;

	ACLASS()
	class EditorMainWindowContainer : public DockableWindowContainer
	{
		GENERATED_BODY()

	private:
		APROPERTY()
		PPtr<MenuItem> m_MenuFile;
		APROPERTY()
		PPtr<MenuItem> m_MenuEdit;

	protected:
		virtual void OnGUI(const RectF& position) override;

	private:
		void DrawMenu(const RectF& position);
	};
}