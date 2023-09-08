// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Layout/SlateVisibility.h"
#include "Layout/SLateRenderTransform.h"
#include "Layout/FlowDirection.h"
#include "Layout/WidgetClipping.h"
#include "Layout/Geometry.h"
#include "Rendering/SlateWindowElementList.h"
#include "Rendering/PaintArgs.h"
#include "Numerics/VectorInterface/Vector.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SDummyAttributeWidget
{
public:
	using This = SDummyAttributeWidget;
	using Super = void;

public:
	struct NDeclarativeAttr
	{
	};
};

class SLATECORE_API SWidget : public SDummyAttributeWidget
{
	SWidget(const SWidget&) = delete;
	SWidget(SWidget&&) = delete;

private:
	ESlateVisibility::Enum Visibility = ESlateVisibility::Visible;
	EFlowDirection FlowDirection = EFlowDirection::LeftToRight;
	EWidgetClipping Clipping = EWidgetClipping::Inherit;

	Vector2 CachedDesiredSize;
	NSlateRenderTransform RenderTransform;
	Vector2 RenderTransformPivot;
	float RenderOpacity = 1.0f;

	bool bEnabled : 1 = true;
	bool bInvalidated : 1 = true;
	bool bHasRenderTransform : 1 = false;

public:
	SWidget();
	virtual ~SWidget() noexcept;

	virtual String ToString() const;
	virtual void Tick(const NGeometry& AllottedGeomtry, const TimeSpan& InDeltaTime);

	int32 Paint(const NPaintArgs& Args, const NGeometry& AllottedGeometry, const Rect& CullingRect, NSlateWindowElementList& OutDrawElements, int32 InLayer, bool bParentEnabled) const;
	Vector2 GetDesiredSize() const { return CachedDesiredSize; }

	void SetVisibility(ESlateVisibility::Enum InVisibility);
	ESlateVisibility::Enum GetVisibility() const { return Visibility; }

	Vector2 GetRenderTransformPivotWithRespectToFlowDirection();
	NSlateRenderTransform GetRenderTransformWithRespectToFlowDirection();
	inline bool HasRenderTransform() { return bHasRenderTransform; }

	void SetEnabled(bool bInEnabled);
	bool IsEnabled() const { return bEnabled; }

	void Invalidate();
	bool IsInvalidated() const { return bInvalidated; }
	void InvalidateLayoutAndVolatility();

	void SetRenderOpacity(float InOpacity);
	float GetRenderOpacity() const { return RenderOpacity; }

public:
	BEGIN_SLATE_ATTRIBUTE(SWidget)
		DECLARE_SLATE_ATTRIBUTE(ESlateVisibility::Enum, Visibility, ESlateVisibility::Visible)
		DECLARE_SLATE_ATTRIBUTE(bool, bEnabled, true)
		DECLARE_SLATE_ATTRIBUTE(float, RenderOpacity, 1.0f)
	END_SLATE_ATTRIBUTE()

	DECLARE_SLATE_CONSTRUCTOR();

protected:
	void CacheDesiredSize();
	virtual Vector2 ComputeDesiredSize() const = 0;

	virtual int32 OnPaint(const NPaintArgs& Args, const NGeometry& AllottedGeometry, const Rect& CullingRect, NSlateWindowElementList& OutDrawElements, int32 InLayer, bool bParentEnabled) const = 0;

	bool IsChildWidgetCulled(const Rect& CullingRect, const NArrangedWidget& ArrangedChild) const;
	bool ShouldBeEnabled(bool bParentEnabled) const;

protected:
	virtual void OnVisibilityChanged([[maybe_unused]] ESlateVisibility::Enum PrevVisibility, [[maybe_unused]] ESlateVisibility::Enum NewVisibility) {}
	virtual void OnEnabled() {}
	virtual void OnDisabled() {}

private:
	void SetDesiredSize(const Vector2& InDesiredSize);
};

namespace DeclarativeSyntaxSupports
{
	template<class T, class U>
	void InvokeConstructorRecursive(T& WidgetInst, U& Args)
	{
		static_assert(std::derived_from<T, SWidget>);

		WidgetInst.Construct(static_cast<typename T::NDeclarativeAttr&>(Args));
		if constexpr (std::same_as<T, SWidget> == false)
		{
			InvokeConstructorRecursive(static_cast<T::Super&>(WidgetInst), Args);
		}
	}
}

template<std::derived_from<SWidget> T, class U>
std::shared_ptr<T> operator <<(std::shared_ptr<T>&& WidgetInstPtr, U&& Args)
	requires requires { static_cast<typename T::NDeclarativeAttr&>(Args); }
{
	DeclarativeSyntaxSupports::InvokeConstructorRecursive(*WidgetInstPtr, Args);
	return std::move(WidgetInstPtr);
}