// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "DeclarativeSyntaxSupports.h"
#include "SlotBase.h"
#include "Layout/LayoutEnums.h"
#include "Layout/SlateRenderTransform.h"
#include "Layout/Geometry.h"
#include "Layout/LayoutImpl.h"

class SSlateWindowElementList;
class SPaintArgs;
class SArrangedChildrens;
class ArrangedWidget;

class GAME_API SWidget : implements SObject
{
	GENERATED_BODY(SWidget)

private:
	ESlateVisibility _Visibility = ESlateVisibility::Visible;
	EFlowDirection _FlowDirection = EFlowDirection::LeftToRight;
	EWidgetClipping _Clipping = EWidgetClipping::Inherit;

	SlateRenderTransform _RenderTransform = SlateRenderTransform::Identity();
	Vector2 _RenderTransformPivot = Vector2::GetZero();
	uint8 _bEnabled : 1 = true;
	uint8 _bHasRenderTransform : 1 = false;
	std::optional<Geometry> _cachedGeometry;

	std::wstring _name;

public:
	SWidget(const std::wstring& name);

	virtual std::wstring ToString(std::wstring_view formatArgs) override;

	int32 Paint(SPaintArgs* paintArgs, const Geometry& allottedGeometry, const Rect& cullingRect, SSlateWindowElementList* drawElements, int32 layer, bool bParentEnabled);
	void ArrangeChildren(SArrangedChildrens* arrangedChildrens, const Geometry& allottedGeometry);
	inline std::wstring GetName() { return _name; }

	virtual void Tick(const Geometry& allottedGeometry, std::chrono::duration<float> deltaTime);
	virtual Vector2 GetDesiredSize();

	Vector2 GetRenderTransformPivotWithRespectToFlowDirection();
	SlateRenderTransform GetRenderTransformWithRespectToFlowDirection();
	inline bool HasRenderTransform() { return _bHasRenderTransform; }
	std::optional<Geometry> GetCachedGeometry() { return _cachedGeometry; }

	bool SendMouseEvent(const Geometry& allottedGeometry, const Vector2N& location, EMouseButton button, EMouseButtonEvent event);
	bool SendKeyboardEvent(const Geometry& allottedGeometry, EKey key, EKeyboardEvent event);

protected:
	virtual int32 OnPaint(SPaintArgs* paintArgs, const Geometry& allottedGeometry, const Rect& cullingRect, SSlateWindowElementList* drawElements, int32 layer, bool bParentEnabled) = 0;
	virtual void OnArrangeChildren(SArrangedChildrens* arrangedChildrens, const Geometry& allottedGeometry) = 0;

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
		DECLARE_SLATE_ATTRIBUTE(std::wstring, Name)
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