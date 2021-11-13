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
class SSlateAnimationPlayer;
class SSlateAnimationContext;

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
	uint8 bMouseHover : 1 = false;

	float RenderOpacity = 1.0f;
	SSlateAnimationPlayer* AnimPlayer = nullptr;
	Vector2 CachedMouseLocation;

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

	bool SendMouseMoved(const Geometry& AllottedGeometry, const Vector2N& Location);
	bool SendMouseWheelScrolled(const Geometry& AllottedGeometry, int32 ScrollDelta);
	bool SendMouseEvent(const Geometry& AllottedGeometry, const Vector2N& Location, EMouseButton Button, EMouseButtonEvent Event);
	bool SendKeyboardEvent(const Geometry& AllottedGeometry, EKey Key, EKeyboardEvent Event);

protected:
	virtual int32 OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SlateWindowElementList& InDrawElements, int32 InLayer, bool bParentEnabled) = 0;
	virtual void OnArrangeChildren(ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry) = 0;

	virtual bool OnReceiveMouseMoved(const Geometry& AllottedGeometry, const Vector2N& Location) = 0;
	virtual bool OnReceiveMouseWheelScrolled(const Geometry& AllottedGeometry, int32 ScrollDelta) = 0;
	virtual bool OnReceiveMouseEvent(const Geometry& AllottedGeometry, const Vector2N& Location, EMouseButton Button, EMouseButtonEvent Event) = 0;
	virtual bool OnReceiveKeyboardEvent(const Geometry& AllottedGeometry, EKey Key, EKeyboardEvent Event) = 0;
	
	bool IsChildWidgetCulled(const Rect& CullingRect, const ArrangedWidget& ArrangedChild);
	bool ShouldBeEnabled(bool bParentEnabled);

public:
	DECLARE_MULTICAST_EVENT(MouseHoveredEvent, bool);
	MouseHoveredEvent MouseHovered;

public:					
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_ATTRIBUTE(ESlateVisibility, Visibility, ESlateVisibility::Visible)
		DECLARE_SLATE_ATTRIBUTE(EFlowDirection, FlowDirection, EFlowDirection::LeftToRight)
		DECLARE_SLATE_ATTRIBUTE(EWidgetClipping, Clipping, EWidgetClipping::Inherit)
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

public:
	bool PlayAnimation(SSlateAnimationContext* Animation);
	void StopAnimations();
	SSlateAnimationPlayer& GetAnimPlayer();
};

template<std::derived_from<SWidget> TSlateClass, class TDeclarativeAttr>
inline TSlateClass* operator <<(TSlateClass* SlateInstance, TDeclarativeAttr&& Attribute)
{
	SlateInstance->Construct(std::move(Attribute));
	return SlateInstance;
}