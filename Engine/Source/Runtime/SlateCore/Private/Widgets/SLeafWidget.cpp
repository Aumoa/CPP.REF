// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Widgets/SLeafWidget.h"

SLeafWidget::SLeafWidget()
{
}

void SLeafWidget::PrepassLayout()
{
	CacheDesiredSize();
}

void SLeafWidget::OnArrangeChildren([[maybe_unused]] NArrangedChildrens& InoutArrangedChildrens, [[maybe_unused]] const NGeometry& AllottedGeometry) const
{
}