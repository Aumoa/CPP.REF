// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "CoreMinimal.h"
#include "SCWApp.h"
#include "Exceptions/UsageException.h"

int main(int Argc, char* Argv[])
{
	PlatformProcess::SetupStacktraceSignals();
	CommandLine::Init(Argc, Argv);

	std::stop_source CancellationTokenSource;
	Console::CancelKeyPressed += [&]()
	{
		CancellationTokenSource.request_stop();
	};

	std::shared_ptr App = NewObject<ASCWApp>();

	try
	{
		return App->RunAsync(CancellationTokenSource.get_token()).GetResult();
	}
	catch (const UsageException&)
	{
		Console::Error.WriteLine(TEXT("Invalid usage."));
		App->PrintUsage(Console::Error);
	}
	catch (const Exception& E)
	{
		Console::Error.WriteLine(TEXT("Error: {}"), E.ToString());
		return 1;
	}
}