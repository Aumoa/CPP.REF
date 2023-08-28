// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Diagnostics/StackTrace.h"
#include "Platform/PlatformProcess.h"
#include "Platform/PlatformLocalization.h"

StackTrace::StackTrace() noexcept
{
}

String StackTrace::ToString() const
{
	std::vector<String> Strings;
	for (auto& Frame : Frames)
	{
		if (Frame.Location.IsEmpty() == false && Frame.Line > 0)
		{
			Strings.emplace_back(String::Format(TEXT("  at {}!{} in {}({})"), Frame.Module, Frame.Description, Frame.Location, Frame.Line));
		}
		else
		{
			Strings.emplace_back(String::Format(TEXT("  at {}!{}"), Frame.Module, Frame.Description));
		}
	}

	return String::Join(TEXT("\n"), Strings);
}

StackTrace StackTrace::Current() noexcept
{
	StackTrace S;
	PlatformProcess::StacktraceCurrent(S.Frames);
	return S;
}

StackTrace StackTrace::FromThread(const Thread& Thrd) noexcept
{
	StackTrace S;
	PlatformProcess::StacktraceFromThread(Thrd.NativeHandle(), S.Frames);
	return S;
}

StackTrace StackTrace::FromException(void* lpExceptionPointers) noexcept
{
	StackTrace S;
	PlatformProcess::StacktraceFromException(lpExceptionPointers, S.Frames);
	return S;
}