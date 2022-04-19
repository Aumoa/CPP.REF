// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Draw/SlateDrawCollector.h"
#include "Draw/IRenderSlateElement.h"

using namespace ::libty;

SSlateDrawCollector::SSlateDrawCollector()
{
}

void SSlateDrawCollector::AddRenderElement(IRenderSlateElement* Element)
{
	RenderElement Elem;
	Elem.ElementType = EElementType::RenderElement;
	//Elem.Element = Element;

	Elements.emplace_back(Elem);
}

void SSlateDrawCollector::PushClipLayer(const Geometry& ClipGeometry)
{
	RenderElement Elem;
	Elem.ElementType = EElementType::PushClipLayer;
	Elem.AllottedGeometry = ClipGeometry;

	Elements.emplace_back(Elem);
}

void SSlateDrawCollector::PopClipLayer()
{
	RenderElement Elem;
	Elem.ElementType = EElementType::PopClipLayer;

	Elements.emplace_back(Elem);
}

void SSlateDrawCollector::SortByLayer()
{
	//auto AscendSorter = [](RenderElement& Lhs, RenderElement& Rhs)
	//{
	//	return Lhs.Element->GetLayer() < Rhs.Element->GetLayer();
	//};

	//auto Beg = Elements.begin();
	//for (auto It = Elements.begin(); It != Elements.end(); ++It)
	//{
	//	if (It->ElementType != EElementType::RenderElement)
	//	{
	//		auto End = It;
	//		if (End != Beg)
	//		{
	//			--End;
	//			size_t NumSorts = End - Beg;
	//			if (End - Beg > 1)
	//			{
	//				std::sort(Beg, End, AscendSorter);
	//			}
	//		}
	//		Beg = It;
	//		std::advance(Beg, 1);
	//	}
	//}
}

void SSlateDrawCollector::FlushElements(std::vector<RenderElement>& SwapElements)
{
	std::swap(SwapElements, Elements);
}