// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GUI/DockableWindowContainer.h"
#include "EditorMainWindowContainer.gen.h"

namespace Ayla
{
	ACLASS()
	class EditorMainWindowContainer : public DockableWindowContainer
	{
		GENERATED_BODY()

	protected:
		virtual void OnGUI(const RectF& position) override;
	};
}