// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.System:CommandLine;

export import Core.Std;
export import :String;

export class CORE_API CommandLine
{
	CommandLine() = delete;

private:
	static std::vector<String> Args;

public:
	static void Init(int Argc, const char* const Argv[]);
	static bool TryGetValue(String InName, String& OutValue);
	static bool Contains(String InName);
};