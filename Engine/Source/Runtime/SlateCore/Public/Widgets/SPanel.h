// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SWidget.h"

class SLATECORE_API SPanel : public SWidget
{
public:
	SPanel();

public:
	BEGIN_SLATE_ATTRIBUTE(SPanel)
	END_SLATE_ATTRIBUTE()

	DECLARE_SLATE_CONSTRUCTOR() {}
};