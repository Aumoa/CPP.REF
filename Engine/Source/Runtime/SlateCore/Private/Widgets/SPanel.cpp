// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Widgets/SPanel.h"

SPanel::SPanel()
{
}

bool SPanel::PrepassLayout()
{
	const size_t Num = NumChildrens();
	for (size_t i = 0; i < Num; ++i)
	{
		SWidget* CurWidget = GetChildrenAt(i);
		if (CurWidget->PrepassLayout())
		{
			InvalidateLayoutAndVolatility();
		}
	}

	return Super::PrepassLayout();
}

void SPanel::Tick(const NGeometry& AllottedGeometry, const TimeSpan& InDeltaTime)
{
	NArrangedChildrens ArrangedChildrens(ESlateVisibility::All);
	OnArrangeChildren(ArrangedChildrens, AllottedGeometry);

	const std::vector<NArrangedWidget>& ArrangedWidgets = ArrangedChildrens.GetWidgets();
	for (size_t ChildIndex = 0; ChildIndex < ArrangedWidgets.size(); ++ChildIndex)
	{
		const NArrangedWidget& CurWidget = ArrangedWidgets[ChildIndex];
		CurWidget.GetWidget()->Tick(CurWidget.GetGeometry(), InDeltaTime);
	}
}