// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "System/Environment.h"
#include "IO/File.h"
#include "System/Path.h"
#include "System/InvalidOperationException.h"

String Environment::EngineDirectory;

void Environment::SetEngineDirectory(String InPath)
{
	EngineDirectory = InPath;
	
	// check directory validation.
	String BuildToolPath = Path::Combine(EngineDirectory, TEXT("Binaries"), TEXT("DotNET"), TEXT("AylaBuildTool.dll"));
	if (File::Exists(BuildToolPath) == false)
	{
		throw InvalidOperationException(String::Format(TEXT("Directory '{}' is not valid. Cannot found 'AylaBuildTool.dll' in directory 'Binaries/DotNET'."), EngineDirectory));
	}
}