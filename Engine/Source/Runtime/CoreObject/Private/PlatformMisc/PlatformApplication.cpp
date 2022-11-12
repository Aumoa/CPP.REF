// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Core.h"
#include "Casts.h"
#include "PlatformMisc/PlatformApplication.h"
#include "Reflection/ConstructorInfo.h"
#include "Logging/LogCategory.h"
#include "Logging/Log.h"
#include "Threading/Thread.h"
#include "PlatformApplication.gen.cpp"

static PlatformApplication* sApp;
static constexpr LogCategory LogWindowsApp(TEXT("LogWindowsApp"));

void Invoke_crash_handler(const Stacktrace& stacktrace, void* exceptionPointer)
{
	sApp->OnApplicationCrashed(stacktrace, exceptionPointer);
}

void Invoke_sigint(int)
{
	sApp->OnApplicationSignalExit();
}

PlatformApplication::PlatformApplication()
	: Super()
{
}

void PlatformApplication::Tick()
{
	if (!_gcTimer.IsRunning())
	{
		_gcTimer.Start();
	}

	if (_gcTimer.GetElapsed() >= 1s)
	{
		GC::Collect();
		_gcTimer.Restart();
	}
}

int32 PlatformApplication::GetPlatformExitCode()
{
	return _platformExitCode.value_or(-1);
}

void PlatformApplication::OnApplicationShutdown() noexcept
{
	RemoveFromRoot();
}

void PlatformApplication::OnApplicationCrashed(const Stacktrace& stacktrace, void* exceptionPointer) noexcept
{
	Log::Fatal(LogWindowsApp, TEXT("Unhandled SEH exception caught.\n{}"), stacktrace.Trace());
	Log::FlushAll();
}

void PlatformApplication::OnApplicationSignalExit() noexcept
{
	RequestExit(0);
}

PlatformApplication* PlatformApplication::GetApp()
{
	return sApp;
}

#if PLATFORM_WINDOWS

#include "PlatformMisc/Windows/WindowsMinimal.h"
#include "PlatformMisc/Windows/WindowsWindow.h"
#include <csignal>

#undef CreateWindow

static LPTOP_LEVEL_EXCEPTION_FILTER sPreviousHandler;

LONG WINAPI WindowsUnhandledExceptionFilter(_In_ struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	if (sApp)
	{
		Invoke_crash_handler(Stacktrace::CaptureException(ExceptionInfo), ExceptionInfo);
	}

	return SHIPPING ? EXCEPTION_CONTINUE_EXECUTION : EXCEPTION_CONTINUE_SEARCH;
}

void PlatformApplication::StartApplicationLoop()
{
	Thread::GetCurrentThread().SetFriendlyName(TEXT("[ApplicationThread]"));

	MSG msg;
	while (true)
	{
		if (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				_platformExitCode = (int32)msg.wParam;
				break;
			}

			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		else
		{
			Tick();
		}

		if (_platformExitCode.has_value())
		{
			break;
		}
	}
}

void PlatformApplication::RequestExit(int32 exitCode)
{
	_platformExitCode = exitCode;
	PostQuitMessage(exitCode);
}

IPlatformWindow* PlatformApplication::CreateWindow(const String& title)
{
	return gcnew WindowsWindow(title);
}

int32 InvokeWindowsStartup(const CommandLineBuilder& builder)
{
	return sApp->Startup(builder);
}

int32 PlatformApplication::GuardedMain(SubclassOf<PlatformApplication> classOf, const CommandLineBuilder& builder)
{
	Core::Initialize();

	sApp = Cast<PlatformApplication>(classOf->GetConstructors()[0]->Invoke());
	sApp->AddToRoot();

	sPreviousHandler = SetUnhandledExceptionFilter(WindowsUnhandledExceptionFilter);
	signal(SIGINT, Invoke_sigint);
	int32 returnCode = InvokeWindowsStartup(builder);

	sApp->OnApplicationShutdown();
	Core::Shutdown();

	return returnCode;
}

#else
#error Not supported platform.
#endif