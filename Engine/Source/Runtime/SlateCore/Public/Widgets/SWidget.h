// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Layout/SlateVisibility.h"
#include "Layout/SLateRenderTransform.h"
#include "Layout/FlowDirection.h"
#include "Layout/WidgetClipping.h"
#include "Layout/Geometry.h"
#include "Layout/ArrangedChildrens.h"
#include "Layout/HorizontalAlignment.h"
#include "Layout/VerticalAlignment.h"
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
	template<class T = void>
	struct NDeclarativeAttr
	{
	};
};

template<class UPanel>
struct NSlotBase
{
	using This = UPanel;
	std::shared_ptr<SWidget> Content;

	DECLARE_SLATE_ATTRIBUTE(NMargin, SlotPadding);
	DECLARE_SLATE_ATTRIBUTE(EHorizontalAlignment, HAlignment, EHorizontalAlignment::Left);
	DECLARE_SLATE_ATTRIBUTE(EVerticalAlignment, VAlignment, EVerticalAlignment::Top);

	UPanel&& operator [](std::shared_ptr<SWidget> InContent)&&
	{
		Content = std::move(InContent);
		return static_cast<UPanel&&>(std::move(*this));
	}

	UPanel& operator [](std::shared_ptr<SWidget> InContent)&
	{
		Content = std::move(InContent);
		return static_cast<UPanel&>(*this);
	}
};

class SLATECORE_API SWidget : public SDummyAttributeWidget, public std::enable_shared_from_this<SWidget>
{
	GENERATED_SLATE_BODY(SWidget)

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
	bool bLayoutInvalidated : 1 = true;
	bool bVolatilityInvalidated : 1 = true;
	bool bHasRenderTransform : 1 = false;

public:
	SWidget();
	virtual ~SWidget() noexcept;

	virtual String ToString() const;
	virtual void Tick(const NGeometry& AllottedGeometry, const TimeSpan& InDeltaTime);
	virtual bool PrepassLayout();

	int32 Paint(const NPaintArgs& Args, const NGeometry& AllottedGeometry, const Rect& CullingRect, NSlateWindowElementList& OutDrawElements, int32 InLayer, bool bParentEnabled);
	Vector2 GetDesiredSize() const { return CachedDesiredSize; }

	void SetVisibility(ESlateVisibility::Enum InVisibility);
	ESlateVisibility::Enum GetVisibility() const { return Visibility; }

	void SetRenderTransform(const NSlateRenderTransform& InTransform);
	NSlateRenderTransform GetRenderTransformWithRespectToFlowDirection();
	inline bool HasRenderTransform() { return bHasRenderTransform; }

	void SetRenderTransformPivot(const Vector2& InPivot);
	Vector2 GetRenderTransformPivotWithRespectToFlowDirection();

	void SetEnabled(bool bInEnabled);
	bool IsEnabled() const { return bEnabled; }

	bool IsInvalidated() const { return bLayoutInvalidated || bVolatilityInvalidated; }
	void InvalidateLayoutAndVolatility();

	void SetRenderOpacity(float InOpacity);
	float GetRenderOpacity() const { return RenderOpacity; }

	void SetFlowDirection(EFlowDirection InFlowDirection);
	EFlowDirection GetFlowDirection() const { return FlowDirection; }

public:
	BEGIN_SLATE_ATTRIBUTE()
		DECLARE_SLATE_ATTRIBUTE(ESlateVisibility::Enum, Visibility, ESlateVisibility::Visible)
		DECLARE_SLATE_ATTRIBUTE(bool, bEnabled, true)
		DECLARE_SLATE_ATTRIBUTE(float, RenderOpacity, 1.0f)
		DECLARE_SLATE_ATTRIBUTE(EFlowDirection, FlowDirection, EFlowDirection::LeftToRight)
	END_SLATE_ATTRIBUTE();

	DECLARE_SLATE_CONSTRUCTOR();

public:
	void CacheDesiredSize();
	virtual Vector2 ComputeDesiredSize() const = 0;

	virtual int32 OnPaint(const NPaintArgs& Args, const NGeometry& AllottedGeometry, const Rect& CullingRect, NSlateWindowElementList& OutDrawElements, int32 InLayer, bool bParentEnabled) const = 0;

	bool IsChildWidgetCulled(const Rect& CullingRect, const NArrangedWidget& ArrangedChild) const;
	bool ShouldBeEnabled(bool bParentEnabled) const;

protected:
	virtual void OnVisibilityChanged([[maybe_unused]] ESlateVisibility::Enum PrevVisibility, [[maybe_unused]] ESlateVisibility::Enum NewVisibility) {}
	virtual void OnEnabled() {}
	virtual void OnDisabled() {}

	virtual void OnArrangeChildren(NArrangedChildrens& InoutArrangedChildrens, const NGeometry& AllottedGeometry) const = 0;

private:
	void SetDesiredSize(const Vector2& InDesiredSize);

protected:
	static constexpr NMargin LayoutPaddingWithFlow(const NMargin& Padding, EFlowDirection LayoutFlow)
	{
		if (LayoutFlow == EFlowDirection::RightToLeft)
		{
			return NMargin(Padding.Right, Padding.Top, Padding.Left, Padding.Bottom);
		}
		else
		{
			return Padding;
		}
	}
};

namespace DeclarativeSyntaxSupports
{
	template<class T>
	struct HasConstructFunction
	{
		template<class U>
		consteval static bool DoTest(void(U::*)(U::template NDeclarativeAttr<>&))
		{
			return std::same_as<T, U>;
		}

		consteval static bool DoTest(void*)
		{
			return false;
		}

		static constexpr bool bValue = DoTest(&T::Construct);
	};

	template<class T, class U>
	void InvokeConstructorRecursive(T& WidgetInst, U& Args)
	{
		static_assert(std::derived_from<T, SWidget>);

		if constexpr (HasConstructFunction<T>::bValue)
		{
			WidgetInst.Construct(reinterpret_cast<typename T::template NDeclarativeAttr<>&>(Args));
		}
		if constexpr (std::same_as<T, SWidget> == false)
		{
			InvokeConstructorRecursive(static_cast<T::Super&>(WidgetInst), Args);
		}
	}
}

template<std::derived_from<SWidget> T, class U>
std::shared_ptr<T> operator <<(std::shared_ptr<T>&& WidgetInstPtr, U&& Args)
{
	DeclarativeSyntaxSupports::InvokeConstructorRecursive(*WidgetInstPtr, Args);
	return std::move(WidgetInstPtr);
}

template<std::derived_from<SWidget> T, class U>
std::shared_ptr<T> operator <<(std::shared_ptr<T>& WidgetInstPtr, U&& Args)
{
	DeclarativeSyntaxSupports::InvokeConstructorRecursive(*WidgetInstPtr, Args);
	return WidgetInstPtr;
}