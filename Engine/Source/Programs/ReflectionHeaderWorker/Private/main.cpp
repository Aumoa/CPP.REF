// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "App.h"

int main(int argc, char* const* argv)
{
	CommandLine::Init(argc, argv);
	auto cts = CancellationTokenSource::Create();
	Console::CancelKeyPressed += [&cts]()
	{
		cts.Cancel();
	};
	return App::Run(cts.GetToken()).GetExitCode();
}