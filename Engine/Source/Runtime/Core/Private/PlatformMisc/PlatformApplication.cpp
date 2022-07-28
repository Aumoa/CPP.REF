// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Core.h"
#include "Casts.h"
#include "PlatformMisc/PlatformApplication.h"
#include "Reflection/ConstructorInfo.h"
#include "Logging/LogCategory.h"
#include "Logging/Log.h"
#include "PlatformApplication.gen.cpp"

PlatformApplication::PlatformApplication()
	: Super()
{
}

#if PLATFORM_WINDOWS

#pragma push_macro("TEXT")
#undef TEXT
#include <Windows.h>
#pragma pop_macro("TEXT")

#include "PlatformMisc/Windows/WindowsWindow.h"

#undef CreateWindow

static PlatformApplication* sApp;
static LPTOP_LEVEL_EXCEPTION_FILTER sPreviousHandler;
static constexpr LogCategory LogWindowsApp(TEXT("LogWindowsApp"));

LONG WINAPI WindowsUnhandledExceptionFilter(_In_ struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	Log::Fatal(LogWindowsApp, TEXT("Unhandled SEH exception caught.\n{}"), Stacktrace::CaptureException(ExceptionInfo).Trace());
	Log::FlushAll();
	return SHIPPING ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_EXECUTION;
}

void PlatformApplication::StartApplicationLoop()
{
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
	}
}

void PlatformApplication::RequestExit(int32 exitCode)
{
	PostQuitMessage(exitCode);
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
	return _platformExitCode;
}

IPlatformWindow* PlatformApplication::CreateWindow(const String& title)
{
	return gcnew WindowsWindow(title);
}

void PlatformApplication::OnApplicationShutdown() noexcept
{
	RemoveFromRoot();
}

int32 InvokeWindowsStartup(const CommandLineBuilder& builder)
{
	int32 __except_code = 0;

	__try
	{
		return sApp->Startup(builder);
	}
	__except (WindowsUnhandledExceptionFilter((__except_code = GetExceptionCode(), GetExceptionInformation())))
	{
		return __except_code;
	}
}

int32 PlatformApplication::GuardedMain(SubclassOf<PlatformApplication> classOf, const CommandLineBuilder& builder)
{
	Core::Initialize();

	sApp = Cast<PlatformApplication>(classOf->GetConstructors()[0]->Invoke());
	sApp->AddToRoot();

	sPreviousHandler = SetUnhandledExceptionFilter(WindowsUnhandledExceptionFilter);
	int32 returnCode = InvokeWindowsStartup(builder);

	sApp->OnApplicationShutdown();
	Core::Shutdown();

	return returnCode;
}

#else
#error Not supported platform.
#endif