// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "DeclarativeSyntaxSupports.h"
#include "SlotBase.h"
#include "Layout/LayoutEnums.h"
#include "Layout/SlateRenderTransform.h"

struct Geometry;
class SSlateWindowElementList;
class SPaintArgs;
class SArrangedChildrens;
class ArrangedWidget;

class SLATECORE_API SWidget : virtual public SObject
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

	std::wstring _name;

public:
	SWidget(const std::wstring& name);

	virtual std::wstring ToString(std::wstring_view formatArgs) const override;

	int32 Paint(SPaintArgs* paintArgs, const Geometry& allottedGeometry, const Rect& cullingRect, SSlateWindowElementList* drawElements, int32 layer, bool bParentEnabled) const;
	void ArrangeChildren(SArrangedChildrens* arrangedChildrens, const Geometry& allottedGeometry) const;
	inline std::wstring GetName() const { return _name; }

	virtual void Tick(const Geometry& allottedGeometry, std::chrono::duration<float> deltaTime);
	virtual Vector2 GetDesiredSize() const;

	Vector2 GetRenderTransformPivotWithRespectToFlowDirection() const;
	SlateRenderTransform GetRenderTransformWithRespectToFlowDirection() const;
	inline bool HasRenderTransform() const { return _bHasRenderTransform; }

protected:
	virtual int32 OnPaint(SPaintArgs* paintArgs, const Geometry& allottedGeometry, const Rect& cullingRect, SSlateWindowElementList* drawElements, int32 layer, bool bParentEnabled) const = 0;
	virtual void OnArrangeChildren(SArrangedChildrens* arrangedChildrens, const Geometry& allottedGeometry) const = 0;

	bool IsChildWidgetCulled(const Rect& cullingRect, const ArrangedWidget& arrangedChild) const;
	bool ShouldBeEnabled(bool bParentEnabled) const;

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

template<std::derived_from<SWidget> TSlateClass, class TDeclarativeAttr>
inline TSlateClass* operator <<(TSlateClass* slateInstance, TDeclarativeAttr&& attribute)
{
	slateInstance->Construct(std::move(attribute));
	return slateInstance;
}