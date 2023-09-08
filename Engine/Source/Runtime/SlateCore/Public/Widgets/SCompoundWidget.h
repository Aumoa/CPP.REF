// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Widgets/SWidget.h"

class SLATECORE_API SCompoundWidget : public SWidget
{
public:
	SCompoundWidget();

public:
	BEGIN_SLATE_ATTRIBUTE(SCompoundWidget)
	END_SLATE_ATTRIBUTE()

	DECLARE_SLATE_CONSTRUCTOR() {}
};