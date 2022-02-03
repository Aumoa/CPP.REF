// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Draw/SlateRenderer.h"
#include "Draw/IRenderSlateElement.h"
#include "Draw/SlateDrawCollector.h"
#include "RHI/RHIInterfaces.h"

GENERATE_BODY(SSlateRenderer)

SSlateRenderer::SSlateRenderer()
{
}

void SSlateRenderer::PopulateCommands(IRHIDeviceContext2D* CommandList, std::vector<SSlateDrawCollector::RenderElement>& Collector)
{
	for (auto& Element : Collector)
	{
		switch (Element.ElementType)
		{
			case SSlateDrawCollector::EElementType::PushClipLayer:
			{
				auto& CachedGeometry = *Element.AllottedGeometry;

				Matrix3x2 Transform = {};
				memcpy(&Transform, &CachedGeometry.GetAccumulatedRenderTransform().M, sizeof(Matrix2x2));
				Transform.V[2] = CachedGeometry.GetAccumulatedRenderTransform().Translation;
				//CommandList->SetTransform(Transform);
				//CommandList->PushAxisAlignedClip(Rect(Vector2::ZeroVector(), CachedGeometry.GetLocalSize()));
			}
			break;
			case SSlateDrawCollector::EElementType::PopClipLayer:
			{
				//CommandList->PopAxisAlignedClip();
			}
			break;
			case SSlateDrawCollector::EElementType::RenderElement:
			{
				auto RenderElement = Element.Element;
				auto CachedGeometry = RenderElement->GetCachedGeometry();

				Matrix3x2 Transform = {};
				memcpy(&Transform, &CachedGeometry.GetAccumulatedRenderTransform().M, sizeof(Matrix2x2));
				Transform.V[2] = CachedGeometry.GetAccumulatedRenderTransform().Translation;
				//CommandList->SetTransform(Transform);
				RenderElement->RenderElement(CommandList, LocalRenderLayout
				{
					.LocalPosition = Vector2::Zero(),
					.LocalSize = CachedGeometry.GetLocalSize()
				});
			}
			break;
		}
	}
}