// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Layout/SBoxPanel.h"

class SLATE_API SVerticalBoxPanel : public SBoxPanel
{
	GENERATED_SLATE_BODY(SVerticalBoxPanel)

public:
	SVerticalBoxPanel();
	virtual ~SVerticalBoxPanel() noexcept override;
};