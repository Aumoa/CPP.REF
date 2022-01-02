// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Draw/SlateRenderer.h"
#include "Draw/IRenderSlateElement.h"
#include "Draw/SlateDrawCollector.h"
#include "RHI/IRHISolidColorBrush.h"
#include "RHI/IRHIDeviceContext2D.h"
#include "RHI/IRHIDevice.h"

GENERATE_BODY(SSlateRenderer)

SSlateRenderer::SSlateRenderer()
{
}

void SSlateRenderer::PopulateCommands(IRHIDeviceContext2D* CommandList, std::vector<IRenderSlateElement*>& Collector)
{
	for (auto& Element : Collector)
	{
		auto CachedGeometry = Element->GetCachedGeometry();

		Matrix3x2 Transform = {};
		memcpy(&Transform, &CachedGeometry.GetAccumulatedRenderTransform().GetMatrix(), sizeof(Matrix2x2));
		Transform.V[2] = CachedGeometry.GetAccumulatedRenderTransform().GetTranslation();
		CommandList->SetTransform(Transform);

		Element->RenderElement(CommandList, LocalRenderLayout
		{
			.LocalPosition = Vector2::ZeroVector(),
			.LocalSize = CachedGeometry.GetLocalSize()
		});
	}
}