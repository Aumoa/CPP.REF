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

LONG NTAPI HandleSEHException(LPEXCEPTION_POINTERS lpExceptionPtr)
{
	Log::Fatal(LogConsoleApplication, TEXT("Unhandled SEH exception caught.\n{}"), Stacktrace::CaptureException(lpExceptionPtr).Trace());
	Log::FlushAll();
	return EXCEPTION_CONTINUE_SEARCH;
}

#endif

int32 ConsoleApplication::GuardedMain(int32 argc, char** argv, ApplicationFactory_t factory, InvokeMain_t invokeMain)
{
	Core::Initialize();

#if PLATFORM_WINDOWS
	SetUnhandledExceptionFilter(HandleSEHException);
#endif

	int32 returnCode = InvokeMain(argc, argv, factory, invokeMain);

	Core::Shutdown();
	return returnCode;
}

int32 ConsoleApplication::InvokeMain(int32 argc, char** argv, ApplicationFactory_t factory, InvokeMain_t invokeMain)
{
	ConsoleApplication* cApp = factory();
	cApp->AddToRoot();
	return invokeMain(cApp, CommandLineBuilder(argc, argv));
}