// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Widgets/SWidget.h"

namespace libty::inline SlateCore
{
	class SWidget;
	class SceneRenderTarget;

	struct PaintArgs
	{
		SRenderContext* const RContext;
		SWidget*		const OwningWidget;
		float			const DeltaTime;
		float			const RenderOpacity;

		inline static PaintArgs InitPaintArgs(SRenderContext* RContext, float InDeltaTime)
		{
			return PaintArgs
			{
				.RContext = RContext,
				.OwningWidget = nullptr,
				.DeltaTime = InDeltaTime,
				.RenderOpacity = 1.0f
			};
		}

		inline PaintArgs WithNewParent(SWidget* InOwningWidget) const
		{
			return PaintArgs
			{
				.RContext = RContext,
				.OwningWidget = InOwningWidget,
				.DeltaTime = DeltaTime,
				.RenderOpacity = RenderOpacity * InOwningWidget->GetRenderOpacity()
			};
		}
	};
}