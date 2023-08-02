// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module AylaHeaderTool:AylaHeaderToolApp;

export import Core;

export class AylaHeaderToolApp
{
	String SourcePath;
	String IncludesPath;

public:
	AylaHeaderToolApp();

	int32 Run();

private:
	static void PrintUsage(TextWriter& Output);

	void ParseCommandLines();
};