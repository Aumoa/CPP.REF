// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Sources/SourceFile.h"

class AylaHeaderToolApp
{
	String SourcePath;
	String IncludesPath;

	std::vector<std::unique_ptr<SourceFile>> Sources;

public:
	AylaHeaderToolApp();

	Task<int32> RunConsoleAsync(std::stop_token InCancellationToken);

private:
	static void PrintUsage(TextWriter& Output);

	void ParseCommandLines();
};