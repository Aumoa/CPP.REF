// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Widgets/Widget.h"
#include "Layout/Geometry.h"
#include "Layout/ArrangedChildrens.h"
#include "Draw/PaintArgs.h"
#include "Draw/SlateWindowElementList.h"

SWidget::SWidget(const std::wstring& name) : Super()
    , _name(name)
{
}

std::wstring SWidget::ToString(std::wstring_view formatArgs) const
{
	return std::format(L"{}({}): [{}] ({})", _name, StringUtils::AsUnicode(typeid(*this).name()), GetDesiredSize().ToString(formatArgs), SlateVisibilityExtensions::ToString(_Visibility));
}

int32 SWidget::Paint(SPaintArgs* paintArgs, const Geometry& allottedGeometry, const Rect& cullingRect, SSlateWindowElementList* drawElements, int32 layer, bool bParentEnabled) const
{
	return OnPaint(paintArgs, allottedGeometry, cullingRect, drawElements, layer, bParentEnabled);
}

void SWidget::ArrangeChildren(SArrangedChildrens* arrangedChildrens, const Geometry& allottedGeometry) const
{
	OnArrangeChildren(arrangedChildrens, allottedGeometry);
}

void SWidget::Tick(const Geometry& allottedGeometry, std::chrono::duration<float> deltaTime)
{
}

Vector2 SWidget::GetDesiredSize() const
{
	return Vector2::GetZero();
}

Vector2 SWidget::GetRenderTransformPivotWithRespectToFlowDirection() const
{
	return _RenderTransformPivot;
}

SlateRenderTransform SWidget::GetRenderTransformWithRespectToFlowDirection() const
{
	check(_bHasRenderTransform);
	return _RenderTransform;
}

void SWidget::SendMouseEvent(EMouseButton button, EMouseButtonEvent event)
{
    // TODO:
    check(false);
}

void SWidget::SendKeyboardEvent(EKey key, EKeyboardEvent event)
{
    // TODO:
    check(false);
}

bool SWidget::IsChildWidgetCulled(const Rect& cullingRect, const ArrangedWidget& arrangedChild) const
{
    // 1) We check if the rendered bounding box overlaps with the culling rect.  Which is so that
    //    a render transformed element is never culled if it would have been visible to the user.
    if (TransformCalculus2D::IsIntersect(cullingRect, arrangedChild.GetGeometry().GetRenderBoundingRect()))
    {
        return false;
    }

    // 2) We also check the layout bounding box to see if it overlaps with the culling rect.  The
    //    reason for this is a bit more nuanced.  Suppose you dock a widget on the screen on the side
    //    and you want have it animate in and out of the screen.  Even though the layout transform 
    //    keeps the widget on the screen, the render transform alone would have caused it to be culled
    //    and therefore not ticked or painted.  The best way around this for now seems to be to simply
    //    check both rects to see if either one is overlapping the culling volume.
    if (TransformCalculus2D::IsIntersect(cullingRect, arrangedChild.GetGeometry().GetLayoutBoundingRect()))
    {
        return false;
    }

    // There's a special condition if the widget's clipping state is set does not intersect with clipping bounds, they in effect
    // will be setting a new culling rect, so let them pass being culling from this step.
    if (arrangedChild.GetWidget()->GetClipping() == EWidgetClipping::ClipToBoundsWithoutIntersection)
    {
        return false;
    }

    return true;
}

bool SWidget::ShouldBeEnabled(bool bParentEnabled) const
{
    return bParentEnabled && IsEnabled();
}

DEFINE_SLATE_CONSTRUCTOR(SWidget, attr)
{
    _name = std::move(attr._Name);
    SetVisibility(attr._Visibility);
    SetFlowDirection(attr._FlowDirection);
    SetClipping(attr._Clipping);
    SetEnabled(attr._bEnabled);
}

void SWidget::SetVisibility(ESlateVisibility visibility)
{
    _Visibility = visibility;
}

ESlateVisibility SWidget::GetVisibility() const
{
    return _Visibility;
}

void SWidget::SetFlowDirection(EFlowDirection flowDirection)
{
    _FlowDirection = flowDirection;
}

EFlowDirection SWidget::GetFlowDirection() const
{
    return _FlowDirection;
}

void SWidget::SetClipping(EWidgetClipping clipping)
{
    _Clipping = clipping;
}

EWidgetClipping SWidget::GetClipping() const
{
    return _Clipping;
}

void SWidget::SetRenderTransform(const SlateRenderTransform& renderTransform)
{
    _bHasRenderTransform = true;
    _RenderTransform = renderTransform;
}

SlateRenderTransform SWidget::GetRenderTransform() const
{
    check(_bHasRenderTransform);
    return _RenderTransform;
}

void SWidget::SetEnabled(bool bEnabled)
{
    _bEnabled = bEnabled;
}

bool SWidget::IsEnabled() const
{
    return _bEnabled;
}