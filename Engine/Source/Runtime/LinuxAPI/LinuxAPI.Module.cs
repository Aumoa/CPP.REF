// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class LinuxAPI : ModuleRules
{
	public LinuxAPI()
	{
		Script.Enabled = true;
		AddPublicIncludePaths("Public");
		AddPrivateIncludePaths("Private");
		AddPublicDependencyModuleNames("ApplicationCore");
		AddPrivateAdditionalLibraries("X11");
	}
}

