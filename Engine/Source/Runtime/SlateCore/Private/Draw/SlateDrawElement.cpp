// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Draw/SlateDrawElement.h"
#include "Draw/SlateWindowElementList.h"
#include "RHI/IRHITextLayout.h"

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
	SlateDrawElement& Element = List.AddUninitialized();
	Element.Type = EElementType::Box;
	Element.Transform = InTransform;
	Element.Layer = InLayer;

	auto& Payload = List.CreateUninitializedPayload<BoxPayload>(Element.SeekPayloadBuf);
	Payload.Brush = InBrush;
	Payload.RenderOpacity = RenderOpacity;

	return Element;
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
		List.AddReferencedObject(InLayout->shared_from_this());
	}

	return Element;
}