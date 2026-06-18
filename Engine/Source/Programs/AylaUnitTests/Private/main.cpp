// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "CoreMinimal.h"
#include "Console.h"
#include "ScriptingTestBootstrap.h"
#include "UnitTestRunner.h"
#include "Tests/ManagedInteropTest.h"
#include "Tests/NumericsTest.h"
#include "Tests/ObjectLifetimeTest.h"
#include <memory>

using namespace Ayla;

Task<int32> MainAsync(int argc, char** argv, std::stop_token cancellationToken)
{
	PLATFORM_UNREFERENCED_PARAMETER(argc);
	PLATFORM_UNREFERENCED_PARAMETER(argv);

	ScriptingTestBootstrap scriptingBootstrap;
	UnitTestRunner runner;
	runner.AddFixture(TEXT("Numerics"), std::make_unique<NumericsTest>());
	runner.AddFixture(TEXT("ManagedInterop"), std::make_unique<ManagedInteropTest>());
	runner.AddFixture(TEXT("ObjectLifetime"), std::make_unique<ObjectLifetimeTest>());

	co_return co_await runner.RunAsync(cancellationToken);
}

int main(int argc, char** argv)
{
	try
	{
		std::stop_source stopSource;
		return MainAsync(argc, argv, stopSource.get_token()).GetResult();
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
