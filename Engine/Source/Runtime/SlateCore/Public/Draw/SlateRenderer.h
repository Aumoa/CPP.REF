// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRHIDeviceContext2D;
interface IRHISolidColorBrush;
interface IRenderSlateElement;
class SSlateDrawCollector;

class SLATECORE_API SSlateRenderer : implements SObject
{
	GENERATED_BODY(SSlateRenderer)

public:
	SSlateRenderer();

	void PopulateCommands(IRHIDeviceContext2D* CommandList, std::vector<IRenderSlateElement*>& Collector);
};