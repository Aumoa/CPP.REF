// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "Widget.h"

class GAME_API SCompoundWidget : public SWidget
{
	GENERATED_BODY(SCompoundWidget)

public:
	SCompoundWidget(const std::wstring& name);

protected:
	virtual int32 OnPaint(SPaintArgs* paintArgs, const Geometry& allottedGeometry, const Rect& cullingRect, SSlateWindowElementList* drawElements, int32 layer, bool bParentEnabled) const override;

	int32 PaintArrangedChildrens(SPaintArgs* paintArgs, SArrangedChildrens* arrangedChildrens, const Geometry& allottedGeometry, const Rect& cullingRect, SSlateWindowElementList* drawElements, int32 layer, bool bParentEnabled) const;

	virtual bool OnReceiveMouseEvent(const Geometry& allottedGeometry, const Vector2N& location, EMouseButton button, EMouseButtonEvent event) override;
	virtual bool OnReceiveKeyboardEvent(const Geometry& allottedGeometry, EKey key, EKeyboardEvent event) override;
};