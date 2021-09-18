// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "SlateMinimal.h"
#include "BoxPanel.h"

class SLATECORE_API SHorizontalBoxPanel : public SBoxPanel
{
	GENERATED_BODY(SHorizontalBoxPanel)

public:
	SHorizontalBoxPanel(const std::wstring& name);
	virtual ~SHorizontalBoxPanel() override;
};