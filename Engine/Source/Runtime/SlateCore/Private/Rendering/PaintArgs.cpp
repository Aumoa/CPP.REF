// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Rendering/PaintArgs.h"
#include "Widgets/SWidget.h"

NPaintArgs NPaintArgs::InitPaintArgs(const SWidget& InOwningWidget, const TimeSpan& InDeltaTime, const std::vector<NGenericPlatformInputEvent>& InInputEvents, std::vector<bool>& InInputEventHandled)
{
	return NPaintArgs
	{
		.OwningWidget = InOwningWidget,
		.ParentWidget = nullptr,
		.DeltaTime = InDeltaTime,
		.RenderOpacity = 1.0f,

		.InputEvents = InInputEvents,
		.InputEventHandled = InInputEventHandled
	};
}

NPaintArgs NPaintArgs::WithNewParent(const SWidget& InOwningWidget) const
{
	return NPaintArgs
	{
		.OwningWidget = InOwningWidget,
		.ParentWidget = &OwningWidget,
		.DeltaTime = DeltaTime,
		.RenderOpacity = RenderOpacity * InOwningWidget.GetRenderOpacity(),

		.InputEvents = InputEvents,
		.InputEventHandled = InputEventHandled
	};
}