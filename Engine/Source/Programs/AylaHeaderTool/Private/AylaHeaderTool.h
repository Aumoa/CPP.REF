// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Projects/HeaderFile.h"
#include "Projects/CSProjectFile.h"

class AylaHeaderTool
{
private:
	String SourceLocation;
	String Build;
	String Output;

	std::vector<HeaderFile> HeaderFiles;
	std::optional<CSProjectFile> CSProject;

public:
	AylaHeaderTool();

	Task<int32> Run(std::stop_token SToken = {});

	inline String GetSourceLocation() const { return SourceLocation; }
	inline String GetBuildLocation() const { return Build; }

private:
	void PrintUsage(TextWriter& Writer);
};