// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "CoreMinimal.h"
#include "Misc/CommandLine.h"
#include "AylaHeaderTool.h"
#include <csignal>

std::stop_source Sigint;

void signal_sigint(int)
{
	Sigint.request_stop();
}

int main(int Argc, char** Argv)
{
	CommandLine::Set(Argc, Argv);

	AylaHeaderTool App;
	signal(SIGINT, signal_sigint);

	return (int)App.Run(Sigint.get_token()).GetResult();
}