// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Widgets/Panel/PanelWidget.h"

SPanelWidget::SPanelWidget(const std::wstring& name) : Super(name)
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}