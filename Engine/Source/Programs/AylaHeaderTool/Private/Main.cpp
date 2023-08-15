// Copyright 2020-2022 Aumoa.lib. All right reserved.

import Core;
import AylaHeaderTool;

int main(int Argc, char* Argv[])
{
	CommandLine::Init(Argc, Argv);

	std::stop_source CancellationTokenSource;
	Console::CancelKeyPressed().AddLambda([&CancellationTokenSource]()
	{
		CancellationTokenSource.request_stop();
	});

	AylaHeaderToolApp App;
	return (int)App.RunConsoleAsync(CancellationTokenSource.get_token()).GetResult();
}