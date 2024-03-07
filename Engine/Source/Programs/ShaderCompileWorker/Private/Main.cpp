// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "CoreMinimal.h"
#include "SCWApp.h"
#include "Exceptions/UsageException.h"
#include "Exceptions/TerminateException.h"

int main(int Argc, char* Argv[])
{
	ThreadPool::Initialize(2, 2);

	PlatformProcess::SetupStacktraceSignals();
	CommandLine::Init(Argc, Argv);

	CancellationTokenSource cancellationTokenSource = std::in_place;
	Console::CancelKeyPressed += [&]()
	{
		cancellationTokenSource.Cancel();
	};

	NSCWApp App;

	return TryFinally([&]()
	{
		try
		{
			App.RunAsync(cancellationTokenSource.GetToken()).GetResult();
			return 0;
		}
		catch (const UsageException&)
		{
			Console::Error.WriteLine(TEXT("Invalid usage."));
			App.PrintUsage(Console::Error);
			return 1;
		}
		catch (const TerminateException& E)
		{
			Console::Error.WriteLine(E.GetMessage());
			return 1;
		}
		catch (const Exception& E)
		{
			Console::Error.WriteLine(TEXT("Error: {}"), E.ToString());
			return 1;
		}
		catch (const std::exception& E)
		{
			Console::Error.WriteLine(TEXT("Unexpected error: {}"), String::FromLiteral(E.what()));
			return 1;
		}
	},
	[&]()
	{
		fflush(stderr);
	}).Execute();
}