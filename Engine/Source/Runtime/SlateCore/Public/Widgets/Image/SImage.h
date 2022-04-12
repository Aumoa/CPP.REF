// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Widgets/SLeafWidget.h"
#include "Draw/SlateBrush.h"

namespace libty::inline SlateCore
{
	class SLATECORE_API SImage : extends(SLeafWidget)
	{
		GENERATED_BODY(SImage);

	private:
		class SRenderElement;

	private:
		//SPROPERTY(ImageSource)
		//IRHIBitmap* ImageSource = nullptr;
		//SPROPERTY(TintBrush)
		//IRHISolidColorBrush* TintBrush = nullptr;

		SPROPERTY(CachedRenderElement)
		SRenderElement* CachedRenderElement = nullptr;

		Vector2 ImageSize;
		Color TintColor;

	public:
		SImage();

		void SetBrush(const SlateBrush& InBrush);
		SlateBrush GetBrush();
		void SetTintColor(const Color& InTintColor);
		Color GetTintColor() const;

	public:
		BEGIN_SLATE_ATTRIBUTE
			DECLARE_SLATE_ATTRIBUTE(SlateBrush, Brush)
			DECLARE_SLATE_ATTRIBUTE(Color, TintColor, NamedColors::White)
		END_SLATE_ATTRIBUTE

		DECLARE_SLATE_CONSTRUCTOR();

	protected:
		virtual Vector2 ComputeDesiredSize() override;
		virtual int32 OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SSlateDrawCollector* DrawCollector, int32 InLayer, bool bParentEnabled) override;
	};
}