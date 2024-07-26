export module Core:CommandLine;

export import :Std;
export import :String;

export class CORE_API CommandLine
{
	CommandLine() = delete;

private:
	static std::vector<String> Args;

public:
	static void Init(int Argc, const char* const Argv[]);
	static void Init(String CmdArgs);
	static bool TryGetValue(String InName, String& OutValue);
	static bool Contains(String InName);
};