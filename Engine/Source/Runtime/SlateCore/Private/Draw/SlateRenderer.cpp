// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Draw/SlateRenderer.h"
#include "Draw/SlateWindowElementList.h"
#include "RHI/IRHISolidColorBrush.h"
#include "RHI/IRHIDeviceContext2D.h"
#include "RHI/IRHIDevice.h"

GENERATE_BODY(SSlateRenderer)

SSlateRenderer::SSlateRenderer()
{
}

void SSlateRenderer::PopulateCommands(IRHIDeviceContext2D* CommandList, SlateWindowElementList& Elements)
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
			auto TintBrush = GetTintBrush(CommandList, Payload.Brush.TintColor, Payload.RenderOpacity);

			if (Payload.Brush.ImageSource)
			{
				Rect Destination(LocalPosition, LocalSize + LocalPosition);
				CommandList->DrawBitmap(Payload.Brush.ImageSource, &Destination, Payload.RenderOpacity);
			}
			else
			{
				CommandList->FillRectangle(TintBrush, Rect(LocalPosition, LocalSize + LocalPosition));
			}
		}
		else if (Element.Type == SlateDrawElement::EElementType::Ellipse)
		{
			const auto& Payload = Element.GetBoxPayload(Elements);
			auto TintBrush = GetTintBrush(CommandList, Payload.Brush.TintColor, Payload.RenderOpacity);

			if (Payload.Brush.ImageSource)
			{
				checkf(false, L"NOT IMPLEMENTED.");
			}
			else
			{
				CommandList->FillEllipse(TintBrush, Rect(LocalPosition, LocalSize + LocalPosition));
			}
		}
		else if (Element.Type == SlateDrawElement::EElementType::Text)
		{
			const auto& Payload = Element.GetTextPayload(Elements);
			if (Payload.Text)
			{
				auto TintBrush = GetTintBrush(CommandList, Payload.TintColor, Payload.RenderOpacity);

				CommandList->DrawTextLayout(LocalPosition, Payload.Text, TintBrush, ERHIDrawTextOptions::None);
			}
		}
	}
}

IRHISolidColorBrush* SSlateRenderer::GetTintBrush(IRHIDeviceContext2D* CommandList, const Color& TintColor, float RenderOpacity)
{
	if (TintBrush == nullptr)
	{
		TintBrush = CommandList->GetDevice()->CreateSolidColorBrush(TintColor, RenderOpacity);
	}
	else
	{
		TintBrush->SetColor(TintColor);
		TintBrush->SetOpacity(RenderOpacity);
	}

	return TintBrush;
}