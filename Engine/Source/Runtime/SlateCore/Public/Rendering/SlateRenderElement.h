// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Layout/PaintGeometry.h"
#include "Rendering/SlateRenderProxy.h"
#include "Numerics/VectorInterface/Color.h"

struct SLATECORE_API NSlateRenderElement
{
	Vector2 AbsolutePosition;
	NPaintGeometry Layout;

	int32 Layer = 0;
	Color TintColor;
	float RenderOpacity = 0;

	std::shared_ptr<NSlateRenderProxy> Proxy;
};