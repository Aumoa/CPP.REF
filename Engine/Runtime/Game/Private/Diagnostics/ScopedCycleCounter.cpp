// Copyright 2020 Aumoa.lib. All right reserved.

#include "Diagnostics/ScopedCycleCounter.h"

#include "Diagnostics/ScopedCycleCounterStorage.h"

#if defined(_DEBUG)
#include "WindowsMinimal.h"
#include "Logging/LogMacros.h"
#endif

ScopedCycleCounter::ScopedCycleCounter(TRefPtr<String> inNamespace, TRefPtr<String> inName) : Super()
{
	nspace = inNamespace;
	name = inName;

#if defined(_DEBUG)
	debugThreadId = (size_t)::GetCurrentThreadId();
#endif

	start = MyTimer::now();
}

ScopedCycleCounter::~ScopedCycleCounter()
{
#if defined(_DEBUG)
	DWORD threadId = ::GetCurrentThreadId();
	if (threadId != debugThreadId) {
		SE_LOG(LogDiagnostics, Warning, L"ScopedCycleCounter is must be measured on the same thread. This will be getting inaccurate result.");
	}
#endif

	auto now = MyTimer::now();
	mydur = now - start;

	RegisterFrameDiagnostics();
}

TRefPtr<String> ScopedCycleCounter::Namespace_get() const
{
	return nspace;
}

TRefPtr<String> ScopedCycleCounter::Name_get() const
{
	return name;
}

Seconds ScopedCycleCounter::Secs_get() const
{
	return mydur;
}

void ScopedCycleCounter::RegisterFrameDiagnostics()
{
	ScopedCycleCounterStorage::AddCounter(*this);
}