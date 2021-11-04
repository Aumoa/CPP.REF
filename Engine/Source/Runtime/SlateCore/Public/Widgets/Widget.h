// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "DeclarativeSyntaxSupports.h"
#include "SlotBase.h"
#include "Layout/Layout.h"

class SlateWindowElementList;
class PaintArgs;
class ArrangedChildrens;
class ArrangedWidget;

class SLATECORE_API SWidget : implements SObject
{
	GENERATED_BODY(SWidget)

private:
	ESlateVisibility Visibility = ESlateVisibility::Visible;
	EFlowDirection FlowDirection = EFlowDirection::LeftToRight;
	EWidgetClipping Clipping = EWidgetClipping::Inherit;

	SlateRenderTransform RenderTransform = SlateRenderTransform::Identity();
	Vector2 RenderTransformPivot = Vector2(0.5f, 0.5f);
	uint8 bEnabled : 1 = true;
	uint8 bHasRenderTransform : 1 = false;
	std::optional<Geometry> CachedGeometry;
	std::optional<Vector2> CachedDesiredSize;

	float RenderOpacity = 1.0f;

public:
	SWidget();

	virtual std::wstring ToString(std::wstring_view InFormatArgs = L"") override;

	int32 Paint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SlateWindowElementList& InDrawElements, int32 InLayer, bool bParentEnabled);
	void ArrangeChildren(ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry);

	virtual void Tick(const Geometry& AllottedGeometry, float InDeltaTime);
	virtual Vector2 GetDesiredSize();

	Vector2 GetRenderTransformPivotWithRespectToFlowDirection();
	SlateRenderTransform GetRenderTransformWithRespectToFlowDirection();
	inline bool HasRenderTransform() { return bHasRenderTransform; }
	std::optional<Geometry> GetCachedGeometry() { return CachedGeometry; }

	bool SendMouseEvent(const Geometry& AllottedGeometry, const Vector2N& Location, EMouseButton Button, EMouseButtonEvent Event);
	bool SendKeyboardEvent(const Geometry& AllottedGeometry, EKey Key, EKeyboardEvent Event);

protected:
	virtual int32 OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SlateWindowElementList& InDrawElements, int32 InLayer, bool bParentEnabled) = 0;
	virtual void OnArrangeChildren(ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry) = 0;

	virtual bool OnReceiveMouseEvent(const Geometry& AllottedGeometry, const Vector2N& Location, EMouseButton Button, EMouseButtonEvent Event) = 0;
	virtual bool OnReceiveKeyboardEvent(const Geometry& AllottedGeometry, EKey Key, EKeyboardEvent Event) = 0;
	
	bool IsChildWidgetCulled(const Rect& CullingRect, const ArrangedWidget& ArrangedChild);
	bool ShouldBeEnabled(bool bParentEnabled);

public:					
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_ATTRIBUTE(ESlateVisibility, Visibility)
		DECLARE_SLATE_ATTRIBUTE(EFlowDirection, FlowDirection)
		DECLARE_SLATE_ATTRIBUTE(EWidgetClipping, Clipping)
		DECLARE_SLATE_ATTRIBUTE(bool, bEnabled)
		DECLARE_SLATE_ATTRIBUTE(float, RenderOpacity, 1.0f)
	END_SLATE_ATTRIBUTE;

	DECLARE_SLATE_CONSTRUCTOR();

	void SetVisibility(ESlateVisibility InVisibility);
	ESlateVisibility GetVisibility();
	void SetFlowDirection(EFlowDirection InFlowDirection);
	EFlowDirection GetFlowDirection();
	void SetClipping(EWidgetClipping InClipping);
	EWidgetClipping GetClipping();

	void SetRenderTransform(const SlateRenderTransform& InRenderTransform);
	SlateRenderTransform GetRenderTransform();
	void SetEnabled(bool bEnabled);
	bool IsEnabled();
	void SetRenderOpacity(float InOpacity);
	float GetRenderOpacity();
};

template<std::derived_from<SWidget> TSlateClass, class TDeclarativeAttr>
inline TSlateClass* operator <<(TSlateClass* SlateInstance, TDeclarativeAttr&& Attribute)
{
	SlateInstance->Construct(std::move(Attribute));
	return SlateInstance;
}