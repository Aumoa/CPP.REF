// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "SlateMinimal.h"
#include "Widgets/CompoundWidget.h"

class SLATECORE_API SPanelWidget : public SCompoundWidget
{
	GENERATED_BODY(SPanelWidget)

public:
	SPanelWidget(const std::wstring& name);
};