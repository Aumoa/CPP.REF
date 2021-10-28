// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CompoundWidget.h"

interface IRHIDevice;

class APPENGINE_API SWindow : public SCompoundWidget
{
	GENERATED_BODY(SWindow)

public:
	SWindow();

	void TickAndPaint(float InDeltaTime, IRHIDevice* Device);
};