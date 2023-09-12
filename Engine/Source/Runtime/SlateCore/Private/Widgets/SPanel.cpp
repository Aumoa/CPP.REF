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

	CacheDesiredSize();
}