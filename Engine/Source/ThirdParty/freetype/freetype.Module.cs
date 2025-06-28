// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class freetype : ModuleRules
{
	public freetype()
	{
		Type = ModuleType.ThirdParty;
		AddPublicIncludePaths(".");
	}
}

