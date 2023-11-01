// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Layout/SBoxPanel.h"

class SLATE_API SHorizontalBoxPanel : public SBoxPanel
{
	GENERATED_SLATE_BODY(SHorizontalBoxPanel)

public:
	SHorizontalBoxPanel();
	virtual ~SHorizontalBoxPanel() noexcept override;
};