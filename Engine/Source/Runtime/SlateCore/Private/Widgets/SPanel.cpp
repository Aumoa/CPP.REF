// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Widgets/SPanel.h"

SPanel::SPanel()
{
}

void SPanel::PrepassLayout()
{
	const size_t Num = NumChildrens();
	for (size_t i = 0; i < Num; ++i)
	{
		GetChildrenAt(i)->PrepassLayout();
	}

	Super::PrepassLayout();
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