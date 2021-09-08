// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "SlateMinimal.h"
#include "Widgets/LeafWidget.h"

class Texture2D;

class GAME_API SImage : public SLeafWidget
{
	GENERATED_BODY(SImage)

private:
	SlateBrush _brush;

public:
	SImage(const std::wstring& name);
	virtual ~SImage() override;

	virtual Vector2 GetDesiredSize() const override;

	void SetBrush(const SlateBrush& brush);
	void SetTexture2D(Texture2D* texture, bool bMatchSize = true);
	SlateBrush GetBrush() const;

public:
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_ATTRIBUTE(SlateBrush, Brush)
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();

protected:
	virtual int32 OnPaint(PaintArgs* paintArgs, const Geometry& allottedGeometry, const Rect& cullingRect, SlateWindowElementList* drawElements, int32 layer, bool bParentEnabled) const override;
};