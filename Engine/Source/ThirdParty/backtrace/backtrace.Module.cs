// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class backtrace : ModuleRules
{
	public backtrace()
	{
		Type = ModuleType.ThirdParty;
		AddPublicIncludePaths(".");
	}
}

