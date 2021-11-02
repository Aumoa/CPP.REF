// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/Widget.h"
#include "Draw/PaintArgs.h"
#include "Draw/SlateWindowElementList.h"

SWidget::SWidget() : Super()
{
}

std::wstring SWidget::ToString(std::wstring_view InFormatArgs)
{
	return std::format(L"{}({}): [{}] ({})", GetName(), GetType()->GetFriendlyName(), GetDesiredSize().ToString(InFormatArgs), SlateVisibilityExtensions::ToString(Visibility));
}

int32 SWidget::Paint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SlateWindowElementList& InDrawElements, int32 InLayer, bool bParentEnabled)
{
	return OnPaint(Args, AllottedGeometry, CullingRect, InDrawElements, InLayer, bParentEnabled);
}

void SWidget::ArrangeChildren(ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry)
{
	OnArrangeChildren(InoutArrangedChildrens, AllottedGeometry);
}

void SWidget::Tick(const Geometry& AllottedGeometry, float InDeltaTime)
{
}

Vector2 SWidget::GetDesiredSize()
{
	return Vector2::ZeroVector();
}

Vector2 SWidget::GetRenderTransformPivotWithRespectToFlowDirection()
{
	return RenderTransformPivot;
}

SlateRenderTransform SWidget::GetRenderTransformWithRespectToFlowDirection()
{
	check(bHasRenderTransform);
	return RenderTransform;
}

bool SWidget::SendMouseEvent(const Geometry& AllottedGeometry, const Vector2N& Location, EMouseButton Button, EMouseButtonEvent Event)
{
    if (SlateVisibilityExtensions::IsHitTestVisible(GetVisibility()) &&
        AllottedGeometry.GetLayoutBoundingRect().PtInRect(Location.Cast<float>()))
    {
        return OnReceiveMouseEvent(AllottedGeometry, Location, Button, Event);
    }

    return false;
}

bool SWidget::SendKeyboardEvent(const Geometry& AllottedGeometry, EKey Key, EKeyboardEvent Event)
{
    if (SlateVisibilityExtensions::IsHitTestVisible(GetVisibility()))
    {
        return OnReceiveKeyboardEvent(AllottedGeometry, Key, Event);
    }

    return false;
}

bool SWidget::IsChildWidgetCulled(const Rect& CullingRect, const ArrangedWidget& ArrangedChild)
{
    // 1) We check if the rendered bounding box overlaps with the culling rect.  Which is so that
    //    a render transformed element is never culled if it would have been visible to the user.
    if (TransformCalculus2D::IsIntersect(CullingRect, ArrangedChild.GetGeometry().GetRenderBoundingRect()))
    {
        return false;
    }

    // 2) We also check the layout bounding box to see if it overlaps with the culling rect.  The
    //    reason for this is a bit more nuanced.  Suppose you dock a widget on the screen on the side
    //    and you want have it animate in and out of the screen.  Even though the layout transform 
    //    keeps the widget on the screen, the render transform alone would have caused it to be culled
    //    and therefore not ticked or painted.  The best way around this for now seems to be to simply
    //    check both rects to see if either one is overlapping the culling volume.
    if (TransformCalculus2D::IsIntersect(CullingRect, ArrangedChild.GetGeometry().GetLayoutBoundingRect()))
    {
        return false;
    }

    // There's a special condition if the widget's clipping state is set does not intersect with clipping bounds, they in effect
    // will be setting a new culling rect, so let them pass being culling from this step.
    if (ArrangedChild.GetWidget()->GetClipping() == EWidgetClipping::ClipToBoundsWithoutIntersection)
    {
        return false;
    }

    return true;
}

bool SWidget::ShouldBeEnabled(bool bParentEnabled)
{
    return bParentEnabled && IsEnabled();
}

DEFINE_SLATE_CONSTRUCTOR(SWidget, Attr)
{
    SetVisibility(Attr._Visibility);
    SetFlowDirection(Attr._FlowDirection);
    SetClipping(Attr._Clipping);
    SetEnabled(Attr._bEnabled);
}

void SWidget::SetVisibility(ESlateVisibility InVisibility)
{
    Visibility = InVisibility;
}

ESlateVisibility SWidget::GetVisibility()
{
    return Visibility;
}

void SWidget::SetFlowDirection(EFlowDirection InFlowDirection)
{
    FlowDirection = InFlowDirection;
}

EFlowDirection SWidget::GetFlowDirection()
{
    return FlowDirection;
}

void SWidget::SetClipping(EWidgetClipping InClipping)
{
    Clipping = InClipping;
}

EWidgetClipping SWidget::GetClipping()
{
    return Clipping;
}

void SWidget::SetRenderTransform(const SlateRenderTransform& InRenderTransform)
{
    bHasRenderTransform = true;
    RenderTransform = InRenderTransform;
}

SlateRenderTransform SWidget::GetRenderTransform()
{
    check(bHasRenderTransform);
    return RenderTransform;
}

void SWidget::SetEnabled(bool bEnabled)
{
    this->bEnabled = bEnabled;
}

bool SWidget::IsEnabled()
{
    return bEnabled;
}