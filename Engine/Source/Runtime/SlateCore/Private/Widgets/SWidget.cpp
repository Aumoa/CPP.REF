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
	return String::Format(TEXT("{}"), String::FromLiteral(typeid(*this).name()));
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
	}
}

void SWidget::Invalidate()
{
	bInvalidated = true;
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

void SWidget::SetDesiredSize(const Vector2& InDesiredSize)
{
	DesiredSize = InDesiredSize;
}