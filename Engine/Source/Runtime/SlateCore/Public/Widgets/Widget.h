// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "Layout/FlowDirection.h"
#include "Layout/SlateRenderTransform.h"
#include "Draw/SlateVisibility.h"
#include "Draw/WidgetClipping.h"

struct Geometry;
class SlateWindowElementList;
class PaintArgs;
class ArrangedChildrens;
class ArrangedWidget;

class SLATECORE_API SWidget : virtual public Object
{
	CLASS_BODY(SWidget)

private:
	ESlateVisibility _Visibility = ESlateVisibility::Visible;
	EFlowDirection _FlowDirection = EFlowDirection::LeftToRight;
	EWidgetClipping _Clipping = EWidgetClipping::Inherit;

	SlateRenderTransform _RenderTransform = SlateRenderTransform::Identity();
	Vector2 _RenderTransformPivot = Vector2::GetZero();
	uint8 _bEnabled : 1 = true;
	uint8 _bHasRenderTransform : 1 = false;

public:
	SWidget();

	virtual std::wstring ToString(std::wstring_view formatArgs) const override;

	int32 Paint(PaintArgs* paintArgs, const Geometry& allottedGeometry, const Rect& cullingRect, SlateWindowElementList* drawElements, int32 layer, bool bParentEnabled) const;
	void ArrangeChildren(ArrangedChildrens* arrangedChildrens, const Geometry& allottedGeometry) const;

	virtual void Tick(const Geometry& allottedGeometry, std::chrono::duration<float> deltaTime);
	virtual Vector2 GetDesiredSize() const;

	Vector2 GetRenderTransformPivotWithRespectToFlowDirection() const;
	SlateRenderTransform GetRenderTransformWithRespectToFlowDirection() const;

protected:
	virtual int32 OnPaint(PaintArgs* paintArgs, const Geometry& allottedGeometry, const Rect& cullingRect, SlateWindowElementList* drawElements, int32 layer, bool bParentEnabled) const = 0;
	virtual void OnArrangeChildren(ArrangedChildrens* arrangedChildrens, const Geometry& allottedGeometry) const = 0;

	bool IsChildWidgetCulled(const Rect& cullingRect, const ArrangedWidget& arrangedChild) const;
	bool ShouldBeEnabled(bool bParentEnabled) const;

public:
	void SetVisibility(ESlateVisibility visibility);
	ESlateVisibility GetVisibility() const;
	void SetFlowDirection(EFlowDirection flowDirection);
	EFlowDirection GetFlowDirection() const;
	void SetClipping(EWidgetClipping clipping);
	EWidgetClipping GetClipping() const;

	void SetRenderTransform(const SlateRenderTransform& renderTransform);
	SlateRenderTransform GetRenderTransform() const;
	void SetEnabled(bool bEnabled);
	bool IsEnabled() const;
};