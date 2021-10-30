// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "BoxPanel.h"

class SHorizontalBoxPanel : public SBoxPanel
{
	GENERATED_BODY(SHorizontalBoxPanel)

public:
	SHorizontalBoxPanel() : Super(EOrientation::Horizontal)
	{
	}
};