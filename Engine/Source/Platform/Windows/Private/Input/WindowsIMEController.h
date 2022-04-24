// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "WindowsIncludes.h"

namespace libty::inline Windows
{
	class SWindowsIMEController : implements(SObject, IPlatformIME)
	{
		GENERATED_BODY(SWindowsIMEController)

	private:
		static SWindowsIMEController* sInstance;

	public:
		SWindowsIMEController();

		static void ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	};
}