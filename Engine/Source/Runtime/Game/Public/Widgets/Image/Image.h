// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/LeafWidget.h"
#include "Draw/SlateBrush.h"

class STexture2D;

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
	SlateBrush GetBrush() const;

public:
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_ATTRIBUTE(SlateBrush, Brush)
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();

protected:
	virtual int32 OnPaint(SPaintArgs* paintArgs, const Geometry& allottedGeometry, const Rect& cullingRect, SSlateWindowElementList* drawElements, int32 layer, bool bParentEnabled) const override;
};