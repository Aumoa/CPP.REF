// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Rendering/PaintArgs.h"
#include "Widgets/SWidget.h"

NPaintArgs NPaintArgs::InitPaintArgs(const SWidget& InOwningWidget, const TimeSpan& InDeltaTime)
{
	return NPaintArgs
	{
		.OwningWidget = InOwningWidget,
		.ParentWidget = nullptr,
		.DeltaTime = InDeltaTime,
		.ParentRenderOpacity = 1.0f
	};
}

NPaintArgs NPaintArgs::WithNewParent(const SWidget& InOwningWidget) const
{
	return NPaintArgs
	{
		.OwningWidget = InOwningWidget,
		.ParentWidget = &OwningWidget,
		.DeltaTime = DeltaTime,
		.ParentRenderOpacity = ParentRenderOpacity * InOwningWidget.GetRenderOpacity()
	};
}