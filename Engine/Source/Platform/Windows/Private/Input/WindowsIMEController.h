// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "WindowsIncludes.h"
#include "Input/IPlatformIME.h"

class SWindowsIMEController : implements SObject, implements IPlatformIME
{
	GENERATED_BODY(SWindowsIMEController)

private:
	static SWindowsIMEController* sInstance;

public:
	SWindowsIMEController();

	static void ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};