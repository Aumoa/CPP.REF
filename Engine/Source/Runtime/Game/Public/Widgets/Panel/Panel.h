// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "SlateMinimal.h"
#include "Widgets/CompoundWidget.h"

class GAME_API SPanel : public SCompoundWidget
{
	GENERATED_BODY(SPanel)

public:
	SPanel(const std::wstring& name);
};