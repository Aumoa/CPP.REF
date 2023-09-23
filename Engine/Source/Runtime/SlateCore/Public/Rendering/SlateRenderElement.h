// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Layout/PaintGeometry.h"
#include "Rendering/SlateRenderProxy.h"

struct SLATECORE_API NSlateRenderElement
{
	Vector2 AbsolutePosition;
	NPaintGeometry Layout;
	std::shared_ptr<NSlateRenderProxy> Proxy;
};