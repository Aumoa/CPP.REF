// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SBoxPanel.h"
#include "SHorizontalBoxPanel.gen.h"

SCLASS()
class GAME_API SHorizontalBoxPanel : public SBoxPanel
{
	GENERATED_BODY()

public:
	SHorizontalBoxPanel();
	virtual ~SHorizontalBoxPanel() noexcept override;
};