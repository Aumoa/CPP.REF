// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Draw/SlateDrawCollector.h"

struct IRHIDeviceContext2D;
struct IRHISolidColorBrush;
struct IRenderSlateElement;

class GAME_API SSlateRenderer : implements SObject
{
	GENERATED_BODY(SSlateRenderer)

public:
	SSlateRenderer();

	void PopulateCommands(IRHIDeviceContext2D* CommandList, std::vector<SSlateDrawCollector::RenderElement>& Collector);
};