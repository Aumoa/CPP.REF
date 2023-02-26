// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "CoreMinimal.h"
#include "Launch.h"

int main(int Argc, char** Argv)
{
	auto PlatformLaunch = NLaunch::GeneratePlatformLaunch(Argc, Argv);
	return PlatformLaunch->GuardedMain([Argv]()
	{
		Console::WriteLine(String::Format(TEXT("AylaHeaderTool {}"), String::FromLiteral(Argv[0])));
		__debugbreak();
		return 0;
	});
}