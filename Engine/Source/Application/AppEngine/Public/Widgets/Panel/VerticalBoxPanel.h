// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "BoxPanel.h"

class SVerticalBoxPanel : public SBoxPanel
{
	GENERATED_BODY(SVerticalBoxPanel)

public:
	SVerticalBoxPanel() : Super(EOrientation::Vertical)
	{
	}
};