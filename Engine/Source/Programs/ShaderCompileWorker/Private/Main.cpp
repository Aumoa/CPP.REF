// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "CoreMinimal.h"

int main(int Argc, char* Argv[])
{
	PlatformProcess::SetupStacktraceSignals();
	try
	{
		Environment::SetEngineDirectory(Environment::GetCurrentDirectory());
	}
	catch (const InvalidOperationException& E)
	{
		Console::Error.WriteLine(TEXT("Working directory is not engine directory. E.Message: {}"), E.GetMessage());
		return 1;
	}

	CommandLine::Init(Argc, Argv);

	std::stop_source CancellationTokenSource;
	Console::CancelKeyPressed += [&CancellationTokenSource]()
	{
		CancellationTokenSource.request_stop();
	};

	try
	{
		Console::WriteLine(TEXT("ShaderCompileWorker"));
		return 0;
	}
	catch (const Exception& E)
	{
		Console::Error.WriteLine(TEXT("Error: {}"), E.ToString());
		return 1;
	}
}