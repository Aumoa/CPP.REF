// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/Panel/PanelWidget.h"

GENERATE_BODY(SPanelWidget);

SPanelWidget::SPanelWidget() : Super()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}