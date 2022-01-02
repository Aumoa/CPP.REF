// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRenderSlateElement;

class SLATECORE_API SSlateDrawCollector : implements SObject
{
	GENERATED_BODY(SSlateDrawCollector)

private:
	SPROPERTY(Elements)
	std::vector<IRenderSlateElement*> Elements;

public:
	SSlateDrawCollector();

	void AddRenderElement(IRenderSlateElement* Element);
	void SortByLayer();
	void FlushElements(std::vector<IRenderSlateElement*>& SwapElements);
};