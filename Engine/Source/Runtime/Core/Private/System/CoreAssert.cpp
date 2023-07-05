// Copyright 2020-2022 Aumoa.lib. All right reserved.

import Core.Std;
import Core.System;
import Core.Diagnostics;

void CoreAssert::Assert(String InExpression, String InMessage, bool bDebugbreak)
{
	Log::Critical(TEXT("{} (in expression: {})\n{}"), InMessage, InExpression, StackTrace::Current().ToString());
#if !SHIPPING
	PlatformMisc::Debugbreak();
#endif
}