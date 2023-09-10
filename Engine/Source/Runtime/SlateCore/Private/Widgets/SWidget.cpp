// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/SWidget.h"
#include "Layout/ArrangedWidget.h"

SWidget::SWidget()
{
}

SWidget::~SWidget() noexcept
{
}

String SWidget::ToString() const
{
	return String::Format(TEXT("{}"), String::FromLiteral(typeid(*this).name()));
}

void SWidget::Tick(const NGeometry& AllottedGeometry, const TimeSpan& InDeltaTime)
{
}

int32 SWidget::Paint(const NPaintArgs& Args, const NGeometry& AllottedGeometry, const Rect& CullingRect, NSlateWindowElementList& OutDrawElements, int32 InLayer, bool bParentEnabled) const
{
	return OnPaint(Args.WithNewParent(*this), AllottedGeometry, CullingRect, OutDrawElements, InLayer, bParentEnabled);
}

void SWidget::SetVisibility(ESlateVisibility::Enum InVisibility)
{
	if (Visibility != InVisibility)
	{
		std::swap(Visibility, InVisibility);
		OnVisibilityChanged(InVisibility, Visibility);
		Invalidate();
	}
}

Vector2 SWidget::GetRenderTransformPivotWithRespectToFlowDirection()
{
	return RenderTransformPivot;
}

NSlateRenderTransform SWidget::GetRenderTransformWithRespectToFlowDirection()
{
	checkf(bHasRenderTransform, TEXT("Widget have not render transform."));
	return RenderTransform;
}

void SWidget::SetEnabled(bool bInEnabled)
{
	if (bEnabled != bInEnabled)
	{
		bEnabled = bInEnabled;
		if (bInEnabled)
		{
			OnEnabled();
		}
		else
		{
			OnDisabled();
		}

		Invalidate();
	}
}

void SWidget::Invalidate()
{
	bInvalidated = true;
}

void SWidget::InvalidateLayoutAndVolatility()
{
	Invalidate();
}

void SWidget::SetRenderOpacity(float InRenderOpacity)
{
	if (RenderOpacity != InRenderOpacity)
	{
		RenderOpacity = InRenderOpacity;
		InvalidateLayoutAndVolatility();
	}
}

void SWidget::SetFlowDirection(EFlowDirection InFlowDirection)
{
	if (FlowDirection != InFlowDirection)
	{
		FlowDirection = InFlowDirection;
		InvalidateLayoutAndVolatility();
	}
}

DEFINE_SLATE_CONSTRUCTOR(SWidget, Attr)
{
	Visibility = Attr._Visibility;
	bEnabled = Attr._bEnabled;
	
	Invalidate();
}

void SWidget::CacheDesiredSize()
{
	SetDesiredSize(ComputeDesiredSize());
}

bool SWidget::IsChildWidgetCulled(const Rect& CullingRect, const NArrangedWidget& ArrangedChild) const
{
	// 1) We check if the rendered bounding box overlaps with the culling rect.  Which is so that
	//    a render transformed element is never culled if it would have been visible to the user.
	if (Rect::IsIntersect(CullingRect, ArrangedChild.GetGeometry().GetRenderBoundingRect()))
	{
		return false;
	}

	// 2) We also check the layout bounding box to see if it overlaps with the culling rect.  The
	//    reason for this is a bit more nuanced.  Suppose you dock a widget on the screen on the side
	//    and you want have it animate in and out of the screen.  Even though the layout transform 
	//    keeps the widget on the screen, the render transform alone would have caused it to be culled
	//    and therefore not ticked or painted.  The best way around this for now seems to be to simply
	//    check both rects to see if either one is overlapping the culling volume.
	if (Rect::IsIntersect(CullingRect, ArrangedChild.GetGeometry().GetLayoutBoundingRect()))
	{
		return false;
	}

	return true;
}

bool SWidget::ShouldBeEnabled(bool bParentEnabled) const
{
	return bParentEnabled && IsEnabled();
}

void SWidget::SetDesiredSize(const Vector2& InDesiredSize)
{
	CachedDesiredSize = InDesiredSize;
}