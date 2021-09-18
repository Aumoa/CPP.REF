// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "SlateMinimal.h"
#include "BoxPanel.h"

class SLATECORE_API SVerticalBoxPanel : public SBoxPanel
{
	GENERATED_BODY(SVerticalBoxPanel)

public:
	SVerticalBoxPanel(const std::wstring& name);
	virtual ~SVerticalBoxPanel() override;
};