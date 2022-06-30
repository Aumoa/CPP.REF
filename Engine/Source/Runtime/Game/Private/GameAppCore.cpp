// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "GameAppCore.h"
#include "Reflection/ConstructorInfo.h"
#include "GameAppCore.gen.cpp"

constexpr LogCategory LogGameApp(TEXT("LogGameApp"));

GameAppCore::GameAppCore() : Super()
{
}

int32 GameAppCore::StartApplication(const CommandLineBuilder& args)
{
	Log::Info(LogGameApp, TEXT("Game Application Started."));
	Log::Info(LogGameApp, TEXT("Game Application Shutdown."));
	return 0;
}

#if PLATFORM_WINDOWS

#pragma push_macro("TEXT")
#undef TEXT
#include <Windows.h>
#pragma pop_macro("TEXT")

INT __stdcall __Handle_SEH_exception(LPEXCEPTION_POINTERS lpExceptionPtr, int32& rv)
{
	Log::Fatal(LogGameApp, TEXT("Unhandled SEH exception caught.\n{}"), Stacktrace::CaptureException(lpExceptionPtr).Trace());
	Log::FlushAll();
	rv = (int32)lpExceptionPtr->ExceptionRecord->ExceptionCode;
	return SHIPPING ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH;
}


int32 __Invoke_platform_main(GameAppCore* self, const CommandLineBuilder& args)
{
	int32 r;
	__try
	{
		return self->StartApplication(args);
	}
	__except (__Handle_SEH_exception(GetExceptionInformation(), r))
	{
	}
	return r;
}

#endif

int32 GameAppCore::GuardedMain(SubclassOf<GameAppCore> applicationClass, const CommandLineBuilder& args)
{
	Core::Initialize();

	auto* ctor = applicationClass->GetConstructors()[0];
	auto* ptr = Cast<GameAppCore>(ctor->Invoke());

	int32 rv = __Invoke_platform_main(ptr, args);

	Core::Shutdown();
	return rv;
}