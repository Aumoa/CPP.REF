// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "SBoxPanel.h"

class GAME_API SHorizontalBoxPanel : public SBoxPanel
{
	using This = SHorizontalBoxPanel;
	using Super = SBoxPanel;

public:
	SHorizontalBoxPanel();
	virtual ~SHorizontalBoxPanel() noexcept override;
};