// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "DeclarativeSyntaxSupport.h"
#include "SlotBase.h"
#include "Layout/Layout.h"
#include "IO/Key.h"
#include "IO/MouseButton.h"
#include "IO/MouseButtonEvent.h"
#include "IO/KeyboardEvent.h"
#include "IO/IMEEvent.h"
#include "IO/MouseState.h"
#include "IO/KeyboardState.h"
#include "Misc/EnableSharedFromThis.h"

class SSlateDrawCollector;
class ArrangedChildrens;
class ArrangedWidget;
struct PaintArgs;

class GAME_API SWidget : public EnableSharedFromThis<SWidget>
{
	using This = SWidget;
	using Super = void;

private:
	SWidget(const SWidget&) = delete;
	SWidget& operator =(const SWidget&) = delete;

private:
	ESlateVisibility::Enum Visibility = ESlateVisibility::Visible;
	EFlowDirection FlowDirection = EFlowDirection::LeftToRight;
	EWidgetClipping Clipping = EWidgetClipping::Inherit;

	SlateRenderTransform RenderTransform = SlateRenderTransform::Identity();
	Vector2 RenderTransformPivot = Vector2(0.5f, 0.5f);
	bool bEnabled : 1 = true;
	bool bHasRenderTransform : 1 = false;
	bool bMouseHover : 1 = false;
	bool bInvalidateLayout : 1 = true;

	float RenderOpacity = 1.0f;
	Vector2 CachedMouseLocation;
	Vector2 CachedDesiredSize;

public:
	SWidget();
	virtual ~SWidget() noexcept;

	virtual String ToString() const;

	int32 Paint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SSlateDrawCollector* DrawCollector, int32 InLayer, bool bParentEnabled);
	void ArrangeChildren(ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry);
	void InvalidateLayoutAndVolatility();

	virtual void Tick(const Geometry& AllottedGeometry, const TimeSpan& InDeltaTime);
	virtual bool PrepassLayout();
	Vector2 GetDesiredSize() const;

	Vector2 GetRenderTransformPivotWithRespectToFlowDirection();
	SlateRenderTransform GetRenderTransformWithRespectToFlowDirection();
	inline bool HasRenderTransform() { return bHasRenderTransform; }

	virtual bool SendMouseMoved(const Geometry& AllottedGeometry, const MouseState& State) = 0;
	virtual bool SendMouseWheelScrolled(const Geometry& AllottedGeometry, int32 ScrollDelta, const MouseState& State) = 0;
	virtual bool SendMouseEvent(const Geometry& AllottedGeometry, EMouseButton Button, EMouseButtonEvent Event, const MouseState& State) = 0;
	virtual bool SendKeyboardEvent(const Geometry& AllottedGeometry, EKey Key, EKeyboardEvent Event, const KeyboardState& State) = 0;
	virtual bool SendIMEEvent(const Geometry& AllottedGeometry, const IMEEvent& EventArgs, const KeyboardState& State) = 0;

protected:
	virtual Vector2 ComputeDesiredSize();

	virtual int32 OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SSlateDrawCollector* DrawCollector, int32 InLayer, bool bParentEnabled) = 0;
	virtual void OnArrangeChildren(ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry) = 0;
	
	bool IsChildWidgetCulled(const Rect& CullingRect, const ArrangedWidget& ArrangedChild);
	bool ShouldBeEnabled(bool bParentEnabled);

	bool ShouldBePrepassLayout();
	void CacheDesiredSize();

public:
	DECLARE_MULTICAST_EVENT(MouseHoveredEvent, bool);
	MouseHoveredEvent MouseHovered;
	DECLARE_MULTICAST_EVENT(VisibilityChangedEvent, ESlateVisibility::Enum);
	VisibilityChangedEvent VisibilityChanged;

public:
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_ATTRIBUTE(ESlateVisibility::Enum, Visibility, ESlateVisibility::Visible)
		DECLARE_SLATE_ATTRIBUTE(EFlowDirection, FlowDirection, EFlowDirection::LeftToRight)
		DECLARE_SLATE_ATTRIBUTE(EWidgetClipping, Clipping, EWidgetClipping::Inherit)
		DECLARE_SLATE_ATTRIBUTE(bool, bEnabled)
		DECLARE_SLATE_ATTRIBUTE(float, RenderOpacity, 1.0f)
	END_SLATE_ATTRIBUTE;

	DECLARE_SLATE_CONSTRUCTOR();

	void SetVisibility(ESlateVisibility::Enum InVisibility);
	ESlateVisibility::Enum GetVisibility();
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

template<class T, class TDeclarativeAttr>
std::shared_ptr<T> operator <<(const std::shared_ptr<T>& ptr, TDeclarativeAttr&& attr) requires
std::derived_from<T, SWidget>
{
	ptr->Construct(std::forward<TDeclarativeAttr>(attr));
	return ptr;
}