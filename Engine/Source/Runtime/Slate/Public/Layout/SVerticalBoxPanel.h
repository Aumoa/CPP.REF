// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Layout/SBoxPanel.h"

class SLATE_API SVerticalBoxPanel : public SBoxPanel
{
	GENERATED_SLATE_BODY(SVerticalBoxPanel)

public:
	SVerticalBoxPanel();
	virtual ~SVerticalBoxPanel() noexcept override;
};