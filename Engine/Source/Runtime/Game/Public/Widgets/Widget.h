// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "DeclarativeSyntaxSupports.h"
#include "SlotBase.h"
#include "Layout/LayoutEnums.h"
#include "Layout/SlateRenderTransform.h"
#include "Layout/Geometry.h"
#include "Layout/LayoutImpl.h"

class SlateWindowElementList;
class PaintArgs;
class ArrangedChildrens;
class ArrangedWidget;

class GAME_API SWidget : implements SObject
{
	GENERATED_BODY(SWidget)

private:
	ESlateVisibility Visibility = ESlateVisibility::Visible;
	EFlowDirection FlowDirection = EFlowDirection::LeftToRight;
	EWidgetClipping Clipping = EWidgetClipping::Inherit;

	SlateRenderTransform RenderTransform = SlateRenderTransform::Identity();
	Vector2 RenderTransformPivot = Vector2::ZeroVector();
	uint8 bEnabled : 1 = true;
	uint8 bHasRenderTransform : 1 = false;
	std::optional<Geometry> CachedGeometry;
	std::optional<Vector2> CachedDesiredSize;

public:
	SWidget();

	virtual std::wstring ToString(std::wstring_view formatArgs) override;

	int32 Paint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SlateWindowElementList& InDrawElements, int32 InLayer, bool bParentEnabled);
	void ArrangeChildren(ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry);

	virtual void Tick(const Geometry& AllottedGeometry, float InDeltaTime);
	virtual Vector2 GetDesiredSize();

	Vector2 GetRenderTransformPivotWithRespectToFlowDirection();
	SlateRenderTransform GetRenderTransformWithRespectToFlowDirection();
	inline bool HasRenderTransform() { return bHasRenderTransform; }
	std::optional<Geometry> GetCachedGeometry() { return CachedGeometry; }

	bool SendMouseEvent(const Geometry& allottedGeometry, const Vector2N& location, EMouseButton button, EMouseButtonEvent event);
	bool SendKeyboardEvent(const Geometry& allottedGeometry, EKey key, EKeyboardEvent event);

protected:
	virtual int32 OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SlateWindowElementList& InDrawElements, int32 InLayer, bool bParentEnabled) = 0;
	virtual void OnArrangeChildren(ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry) = 0;

	virtual bool OnReceiveMouseEvent(const Geometry& allottedGeometry, const Vector2N& location, EMouseButton button, EMouseButtonEvent event) = 0;
	virtual bool OnReceiveKeyboardEvent(const Geometry& allottedGeometry, EKey key, EKeyboardEvent event) = 0;
	
	bool IsChildWidgetCulled(const Rect& cullingRect, const ArrangedWidget& arrangedChild);
	bool ShouldBeEnabled(bool bParentEnabled);

public:					
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_ATTRIBUTE(ESlateVisibility, Visibility)
		DECLARE_SLATE_ATTRIBUTE(EFlowDirection, FlowDirection)
		DECLARE_SLATE_ATTRIBUTE(EWidgetClipping, Clipping)
		DECLARE_SLATE_ATTRIBUTE(bool, bEnabled)
	END_SLATE_ATTRIBUTE;

	DECLARE_SLATE_CONSTRUCTOR();

	void SetVisibility(ESlateVisibility visibility);
	ESlateVisibility GetVisibility();
	void SetFlowDirection(EFlowDirection flowDirection);
	EFlowDirection GetFlowDirection();
	void SetClipping(EWidgetClipping clipping);
	EWidgetClipping GetClipping();

	void SetRenderTransform(const SlateRenderTransform& renderTransform);
	SlateRenderTransform GetRenderTransform();
	void SetEnabled(bool bEnabled);
	bool IsEnabled();
};

template<std::derived_from<SWidget> TSlateClass, class TDeclarativeAttr>
inline TSlateClass* operator <<(TSlateClass* slateInstance, TDeclarativeAttr&& attribute)
{
	slateInstance->Construct(std::move(attribute));
	return slateInstance;
}