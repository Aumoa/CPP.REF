// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "CoreMinimal.h"
#include "Misc/CommandLine.h"
#include "AylaHeaderTool.h"

int main(int Argc, char** Argv)
{
	CommandLine::Set(Argc, Argv);
	AylaHeaderTool App;
	return (int)App.Run();
}