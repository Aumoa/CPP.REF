// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/SWidget.h"

SWidget::SWidget()
{
}

SWidget::~SWidget() noexcept
{
}

String SWidget::ToString() const
{
	return String::Format(TEXT("{}: [{}] ({})"), String::FromLiteral(typeid(*this).name()), GetDesiredSize().ToString(), ESlateVisibility::ToString(Visibility));
}

int32 SWidget::Paint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SSlateDrawCollector* DrawCollector, int32 InLayer, bool bParentEnabled)
{
 //   if (Clipping == EWidgetClipping::ClipToBounds)
 //   {
 //       //DrawCollector->PushClipLayer(AllottedGeometry);
 //   }

 //   PaintArgs NewArgs = Args.WithNewParent(this);
    const PaintArgs& NewArgs = Args;
    int32 Layer = OnPaint(NewArgs, AllottedGeometry, CullingRect, DrawCollector, InLayer, bParentEnabled);

 //   if (Clipping == EWidgetClipping::ClipToBounds)
 //   {
 //       //DrawCollector->PopClipLayer();
 //   }

    return Layer;
}

void SWidget::ArrangeChildren(ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry)
{
	OnArrangeChildren(InoutArrangedChildrens, AllottedGeometry);
}

void SWidget::InvalidateLayoutAndVolatility()
{
    bInvalidateLayout = true;
}

void SWidget::Tick(const Geometry& AllottedGeometry, const TimeSpan& InDeltaTime)
{
    if (ESlateVisibility::IsHitTestVisible(GetVisibility()) &&
        AllottedGeometry.GetRenderBoundingRect().PtInRect(CachedMouseLocation))
    {
        const bool bHover = AllottedGeometry.IsUnderLocation(CachedMouseLocation);
        if (!bMouseHover && bHover)
        {
            MouseHovered.Broadcast(true);
            bMouseHover = true;
        }
        else if (bMouseHover && !bHover)
        {
            MouseHovered.Broadcast(false);
            bMouseHover = false;
        }
    }
    else if (bMouseHover)
    {
        MouseHovered.Broadcast(false);
        bMouseHover = false;
    }
}

bool SWidget::PrepassLayout()
{
    bInvalidateLayout = false;
    return false;
}

Vector2 SWidget::GetDesiredSize() const
{
    return CachedDesiredSize;
}

Vector2 SWidget::GetRenderTransformPivotWithRespectToFlowDirection()
{
	return RenderTransformPivot;
}

SlateRenderTransform SWidget::GetRenderTransformWithRespectToFlowDirection()
{
	checkf(bHasRenderTransform, TEXT("Widget have not render transform."));
	return RenderTransform;
}

Vector2 SWidget::ComputeDesiredSize()
{
    return Vector2::Zero();
}

bool SWidget::IsChildWidgetCulled(const Rect& CullingRect, const ArrangedWidget& ArrangedChild)
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

bool SWidget::ShouldBeEnabled(bool bParentEnabled)
{
    return bParentEnabled && IsEnabled();
}

bool SWidget::ShouldBePrepassLayout()
{
    return bInvalidateLayout;
}

void SWidget::CacheDesiredSize()
{
    CachedDesiredSize = ComputeDesiredSize();
}

DEFINE_SLATE_CONSTRUCTOR(SWidget, Attr)
{
    SetVisibility(Attr._Visibility);
    SetFlowDirection(Attr._FlowDirection);
    SetClipping(Attr._Clipping);
    SetEnabled(Attr._bEnabled);
    SetRenderOpacity(Attr._RenderOpacity);
}

void SWidget::SetVisibility(ESlateVisibility::Enum InVisibility)
{
    if (InVisibility != Visibility)
    {
        Visibility = InVisibility;
        VisibilityChanged.Broadcast(InVisibility);
    }
}

ESlateVisibility::Enum SWidget::GetVisibility()
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
    checkf(bHasRenderTransform, TEXT("Widget have not render transform."));
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

void SWidget::SetRenderOpacity(float InOpacity)
{
    RenderOpacity = InOpacity;
}

float SWidget::GetRenderOpacity()
{
    return RenderOpacity;
}