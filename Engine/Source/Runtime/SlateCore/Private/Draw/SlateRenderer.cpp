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
				memcpy(&Transform, &CachedGeometry.GetAccumulatedRenderTransform().GetMatrix(), sizeof(Matrix2x2));
				Transform.V[2] = CachedGeometry.GetAccumulatedRenderTransform().GetTranslation();
				CommandList->SetTransform(Transform);
				CommandList->PushAxisAlignedClip(Rect(Vector2::ZeroVector(), CachedGeometry.GetLocalSize()));
			}
			break;
			case SSlateDrawCollector::EElementType::PopClipLayer:
			{
				CommandList->PopAxisAlignedClip();
			}
			break;
			case SSlateDrawCollector::EElementType::RenderElement:
			{
				auto RenderElement = Element.Element;
				auto CachedGeometry = RenderElement->GetCachedGeometry();

				Matrix3x2 Transform = {};
				memcpy(&Transform, &CachedGeometry.GetAccumulatedRenderTransform().GetMatrix(), sizeof(Matrix2x2));
				Transform.V[2] = CachedGeometry.GetAccumulatedRenderTransform().GetTranslation();
				CommandList->SetTransform(Transform);

				RenderElement->RenderElement(CommandList, LocalRenderLayout
				{
					.LocalPosition = Vector2::ZeroVector(),
					.LocalSize = CachedGeometry.GetLocalSize()
				});
			}
			break;
		}
	}
}