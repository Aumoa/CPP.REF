// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Draw/SlateDrawCollector.h"
#include "Draw/IRenderSlateElement.h"

GENERATE_BODY(SSlateDrawCollector);

SSlateDrawCollector::SSlateDrawCollector()
{
}

void SSlateDrawCollector::AddRenderElement(IRenderSlateElement* Element)
{
	Elements.emplace_back(Element);
}

void SSlateDrawCollector::SortByLayer()
{
	auto AscendSorter = [](IRenderSlateElement* Lhs, IRenderSlateElement* Rhs)
	{
		return Lhs->GetLayer() < Rhs->GetLayer();
	};

	std::sort(Elements.begin(), Elements.end(), AscendSorter);
}

void SSlateDrawCollector::FlushElements(std::vector<IRenderSlateElement*>& SwapElements)
{
	std::swap(SwapElements, Elements);
}