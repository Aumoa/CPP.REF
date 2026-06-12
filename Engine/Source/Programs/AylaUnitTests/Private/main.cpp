// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "CoreMinimal.h"
#include "Console.h"
#include "UnitTestRunner.h"
#include "Tests/NumericsTest.h"
#include <csignal>
#include <memory>

using namespace Ayla;

std::stop_source g_StopSource;

void HandleSignal(int)
{
	g_StopSource.request_stop();
}

Task<int32> MainAsync(int argc, char** argv, std::stop_token cancellationToken)
{
	PLATFORM_UNREFERENCED_PARAMETER(argc);
	PLATFORM_UNREFERENCED_PARAMETER(argv);

	UnitTestRunner runner;
	runner.AddFixture(TEXT("Numerics"), std::make_unique<NumericsTest>());

	co_return co_await runner.RunAsync(cancellationToken);
}

int main(int argc, char** argv)
{
	signal(SIGINT, HandleSignal);

	try
	{
		return MainAsync(argc, argv, g_StopSource.get_token()).GetResult();
	}
	catch (const Exception& ex)
	{
		Console::Error.WriteLine(TEXT("Unhandled Ayla exception: {}"), ex);
		return 1;
	}
	catch (const std::exception& ex)
	{
		Console::Error.WriteLine(TEXT("Unhandled standard exception: {}"), String::FromLiteral(ex.what()));
		return 1;
	}
	catch (...)
	{
		Console::Error.WriteLine(TEXT("Unhandled unknown exception."));
		return 1;
	}
}
