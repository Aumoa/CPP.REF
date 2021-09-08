// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Widgets/Panel/Panel.h"

SPanel::SPanel(const std::wstring& name) : Super(name)
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}