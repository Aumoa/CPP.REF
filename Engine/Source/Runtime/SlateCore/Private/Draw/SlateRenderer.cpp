// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Draw/SlateRenderer.h"
#include "Draw/SlateWindowElementList.h"
#include "RHI/IRHISolidColorBrush.h"
#include "RHI/IRHIDeviceContext2D.h"
#include "RHI/IRHIDevice.h"

SlateRenderer::SlateRenderer(IRHIDeviceContext2D* CommandList) : CommandList(CommandList)
{
}

SlateRenderer::~SlateRenderer()
{
}

void SlateRenderer::PopulateCommands(SlateWindowElementList& Elements)
{
	Elements.SortByLayer();

	for (auto& Element : Elements.GetElements())
	{
		Matrix3x2 Transform = {};
		memcpy(&Transform, &Element.Transform.GetAccumulatedRenderTransform().GetMatrix(), sizeof(Matrix2x2));
		Transform.V[2] = Element.Transform.GetAccumulatedRenderTransform().GetTranslation();
		CommandList->SetTransform(Transform);

		const Vector2 LocalPosition = Vector2::ZeroVector();
		const Vector2 LocalSize = Element.Transform.GetLocalSize();

		if (Element.Type == SlateDrawElement::EElementType::Box)
		{
			const auto& Payload = Element.GetBoxPayload(Elements);
			auto TintBrush = GetTintBrush(Payload.Brush.TintColor, Payload.RenderOpacity);

			CommandList->FillRectangle(TintBrush, Rect(LocalPosition, LocalSize + LocalPosition));
		}
		else if (Element.Type == SlateDrawElement::EElementType::Text)
		{
			const auto& Payload = Element.GetTextPayload(Elements);
			auto TintBrush = GetTintBrush(Payload.TintColor, Payload.RenderOpacity);

			CommandList->DrawTextLayout(LocalPosition, Payload.Text, TintBrush, ERHIDrawTextOptions::None);
		}
	}
}

IRHISolidColorBrush* SlateRenderer::GetTintBrush(const Color& TintColor, float RenderOpacity)
{
	if (TintBrush == nullptr)
	{
		TintBrush = std::dynamic_pointer_cast<IRHISolidColorBrush>(CommandList->GetDevice()->CreateSolidColorBrush(TintColor, RenderOpacity)->shared_from_this());
		TintBrush->SetOuter(nullptr);
	}
	else
	{
		TintBrush->SetColor(TintColor);
		TintBrush->SetOpacity(RenderOpacity);
	}

	return TintBrush.get();
}