// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "WindowContainer.h"
#include "DockableWindowContainer.gen.h"

namespace Ayla
{
	ACLASS()
	class COREED_API DockableWindowContainer : public WindowContainer
	{
		GENERATED_BODY()

	protected:
		virtual void OnGUI(const RectF& position, RPtr<GUI> gui) override;
	};
}