// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class LinuxAPI : ModuleRules
{
	public LinuxAPI()
	{
		AddPublicIncludePaths("Public");
		AddPrivateIncludePaths("Private");
		AddPublicDependencyModuleNames("Launch", "RenderCore");
	}
}

