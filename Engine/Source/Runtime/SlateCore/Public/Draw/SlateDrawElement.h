// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SlateBrush.h"
#include "SlateFont.h"
#include "Layout/Layout.h"

interface IRHITextLayout;
class SlateWindowElementList;

struct SLATECORE_API SlateDrawElement
{
	enum class EElementType
	{
		Box,
		Ellipse,
		Text
	};

	EElementType Type = EElementType::Box;
	PaintGeometry Transform;
	int32 Layer = 0;
	size_t SeekPayloadBuf = 0;

	struct BoxPayload
	{
		SlateBrush Brush;
		float RenderOpacity;
	};

	struct TextPayload
	{
		IRHITextLayout* Text;
		Color TintColor;
		float RenderOpacity;
	};

	SlateDrawElement();

	const BoxPayload& GetBoxPayload(SlateWindowElementList& List) const;
	const TextPayload& GetTextPayload(SlateWindowElementList& List) const;

	static SlateDrawElement& MakeBox(SlateWindowElementList& List, const SlateBrush& InBrush, float RenderOpacity, const PaintGeometry& InTransform, int32 InLayer);
	static SlateDrawElement& MakeEllipse(SlateWindowElementList& List, const SlateBrush& InBrush, float RenderOpacity, const PaintGeometry& InTransform, int32 InLayer);
	static SlateDrawElement& MakeText(SlateWindowElementList& List, const Color& TintColor, IRHITextLayout* InLayout, float RenderOpacity, const PaintGeometry& InTransform, int32 InLayer);

private:
	template<EElementType Shape>
	static SlateDrawElement& MakeShape(SlateWindowElementList& List, const SlateBrush& InBrush, float RenderOpacity, const PaintGeometry& InTransform, int32 InLayer);
};