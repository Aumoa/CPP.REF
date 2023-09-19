// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "CoreMinimal.h"
#include "AylaHeaderToolApp.h"

int main(int Argc, char* Argv[])
{
	PlatformProcess::SetupStacktraceSignals();
	CommandLine::Init(Argc, Argv);

	std::stop_source CancellationTokenSource;
	Console::CancelKeyPressed += [&]()
	{
		CancellationTokenSource.request_stop();
	};

	try
	{
		AylaHeaderToolApp App;
		return (int)App.RunConsoleAsync(CancellationTokenSource.get_token()).GetResult();
	}
	catch (const Exception& E)
	{
		Console::Error.WriteLine(TEXT("Error: {}"), E.ToString());
		return 1;
	}
}