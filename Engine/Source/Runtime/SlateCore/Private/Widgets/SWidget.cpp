// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/SWidget.h"

SWidget::SWidget()
{
}

SWidget::~SWidget() noexcept
{
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

void SWidget::CacheDesiredSize()
{
	SetDesiredSize(ComputeDesiredSize());
}

void SWidget::SetDesiredSize(const Vector2& InDesiredSize)
{
	DesiredSize = InDesiredSize;
}