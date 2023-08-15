// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module AylaHeaderTool:AylaHeaderToolApp;

export import Core;
export import :SourceFile;

export class AylaHeaderToolApp
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