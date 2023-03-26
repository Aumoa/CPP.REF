// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class CSProjectFile
{
	String ProjectPath;
	String AssemblyName;

public:
	CSProjectFile(String InPath, String AssemblyName);

	Task<> GenerateProjectFileAsync(String BuildOutputPath, std::stop_token SToken = {});
};