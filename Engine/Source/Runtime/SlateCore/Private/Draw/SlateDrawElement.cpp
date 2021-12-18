// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Draw/SlateDrawElement.h"
#include "Draw/SlateWindowElementList.h"
#include "RHI/IRHITextLayout.h"
#include "RHI/IRHIBitmap.h"

SlateDrawElement::SlateDrawElement()
{
}

auto SlateDrawElement::GetBoxPayload(SlateWindowElementList& List) const -> const BoxPayload&
{
	const uint8* Buf = List.GetPayloadBuffer() + SeekPayloadBuf;
	return reinterpret_cast<const BoxPayload&>(*Buf);
}

auto SlateDrawElement::GetTextPayload(SlateWindowElementList& List) const -> const TextPayload&
{
	const uint8* Buf = List.GetPayloadBuffer() + SeekPayloadBuf;
	return reinterpret_cast<const TextPayload&>(*Buf);
}

SlateDrawElement& SlateDrawElement::MakeBox(SlateWindowElementList& List, const SlateBrush& InBrush, float RenderOpacity, const PaintGeometry& InTransform, int32 InLayer)
{
	return MakeShape<EElementType::Box>(List, InBrush, RenderOpacity, InTransform, InLayer);
}

SlateDrawElement& SlateDrawElement::MakeEllipse(SlateWindowElementList& List, const SlateBrush& InBrush, float RenderOpacity, const PaintGeometry& InTransform, int32 InLayer)
{
	return MakeShape<EElementType::Ellipse>(List, InBrush, RenderOpacity, InTransform, InLayer);
}

SlateDrawElement& SlateDrawElement::MakeText(SlateWindowElementList& List, const Color& TintColor, IRHITextLayout* InLayout, float RenderOpacity, const PaintGeometry& InTransform, int32 InLayer)
{
	SlateDrawElement& Element = List.AddUninitialized();
	Element.Type = EElementType::Text;
	Element.Transform = InTransform;
	Element.Layer = InLayer;

	auto& Payload = List.CreateUninitializedPayload<TextPayload>(Element.SeekPayloadBuf);
	Payload.Text = InLayout;
	Payload.TintColor = TintColor;
	Payload.RenderOpacity = RenderOpacity;

	if (InLayout)
	{
		List.AddReferencedObject(InLayout);
	}

	return Element;
}

template<SlateDrawElement::EElementType Shape>
SlateDrawElement& SlateDrawElement::MakeShape(SlateWindowElementList& List, const SlateBrush& InBrush, float RenderOpacity, const PaintGeometry& InTransform, int32 InLayer)
{
	SlateDrawElement& Element = List.AddUninitialized();
	Element.Type = Shape;
	Element.Transform = InTransform;
	Element.Layer = InLayer;

	auto& Payload = List.CreateUninitializedPayload<BoxPayload>(Element.SeekPayloadBuf);
	Payload.Brush = InBrush;
	Payload.RenderOpacity = RenderOpacity;

	if (InBrush.ImageSource)
	{
		List.AddReferencedObject(InBrush.ImageSource);
	}

	return Element;
}