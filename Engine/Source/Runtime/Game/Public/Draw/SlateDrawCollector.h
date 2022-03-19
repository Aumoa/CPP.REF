// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IRenderSlateElement.h"
#include "Layout/Layout.h"

struct IRenderSlateElement;

class GAME_API SSlateDrawCollector : implements SObject
{
	GENERATED_BODY(SSlateDrawCollector)

public:
	SENUM(EElementType, int32,
		RenderElement,,
		PushClipLayer,,
		PopClipLayer,
	);

	struct RenderElement
	{
		GENERATED_STRUCT_BODY(RenderElement)

	public:
		EElementType ElementType;

		SPROPERTY(Element)
		IRenderSlateElement* Element = nullptr;
		std::optional<Geometry> AllottedGeometry;
	};

private:
	SPROPERTY(Elements)
	std::vector<RenderElement> Elements;

public:
	SSlateDrawCollector();

	void AddRenderElement(IRenderSlateElement* Element);
	void PushClipLayer(const Geometry& ClipGeometry);
	void PopClipLayer();

	void SortByLayer();
	void FlushElements(std::vector<RenderElement>& SwapElements);
};