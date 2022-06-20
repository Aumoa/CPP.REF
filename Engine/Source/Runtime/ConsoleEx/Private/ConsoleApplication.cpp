// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "ConsoleApplication.h"
#include "Core.h"
#include "Misc/CommandLineBuilder.h"
#include "Logging/LogCategory.h"
#include "Logging/Log.h"
#include "ConsoleApplication.gen.cpp"

#if PLATFORM_WINDOWS

#pragma push_macro("TEXT")
#undef TEXT
#include <Windows.h>
#pragma pop_macro("TEXT")

#endif

constexpr LogCategory LogConsoleApplication(TEXT("LogConsoleApplication"));

ConsoleApplication::ConsoleApplication()
	: Super()
{
}

ConsoleApplication::~ConsoleApplication() noexcept
{
}

#if PLATFORM_WINDOWS

INT __stdcall HandleSEHException(LPEXCEPTION_POINTERS lpExceptionPtr)
{
	Log::Fatal(LogConsoleApplication, TEXT("Unhandled SEH exception caught.\n{}"), Stacktrace::CaptureException(lpExceptionPtr).Trace());
	Log::FlushAll();
	return SHIPPING ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH;
}

#endif

int32 ConsoleApplication::GuardedMain(int32 argc, char** argv, ApplicationFactory_t factory, InvokeMain_t invokeMain)
{
	Core::Initialize();

	int32 returnCode = 0;
	ConsoleApplication* ptr = nullptr;

#if PLATFORM_WINDOWS

	__try
	{
		returnCode = InvokeMain(argc, argv, factory, invokeMain);
	}
	__except (HandleSEHException(GetExceptionInformation()))
	{
	}

#endif

	Core::Shutdown();
	return returnCode;
}

int32 ConsoleApplication::InvokeMain(int32 argc, char** argv, ApplicationFactory_t factory, InvokeMain_t invokeMain)
{
	try
	{
		return invokeMain(factory(), CommandLineBuilder(argc, argv));
	}
	catch (const Exception& e)
	{
		Log::Fatal(LogConsoleApplication, TEXT("Unhandled exception caught. {}"), e);
		throw;
	}
}