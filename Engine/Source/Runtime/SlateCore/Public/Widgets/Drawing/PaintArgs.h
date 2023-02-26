// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SWidget.h"

struct PaintArgs
{
	std::shared_ptr<SWidget> OwningWidget;
	const TimeSpan DeltaTime;
	const float RenderOpacity;
	RHICommandList* const CommandList;

	static PaintArgs NewArgs(std::shared_ptr<SWidget> owningWidget, const TimeSpan& deltaTime, RHICommandList* commandList)
	{
		return PaintArgs
		{
			.OwningWidget = owningWidget,
			.DeltaTime = deltaTime,
			.RenderOpacity = owningWidget->GetRenderOpacity(),
			.CommandList = commandList
		};
	}

	PaintArgs WithNewParent(std::shared_ptr<SWidget> owningWidget)
	{
		return PaintArgs
		{
			.OwningWidget = owningWidget,
			.DeltaTime = DeltaTime,
			.RenderOpacity = RenderOpacity * owningWidget->GetRenderOpacity(),
			.CommandList = CommandList
		};
	}
};