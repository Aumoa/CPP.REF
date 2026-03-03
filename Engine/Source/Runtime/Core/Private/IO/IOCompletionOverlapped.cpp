// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "IO/IOCompletionOverlapped.h"

namespace Ayla
{
	size_t IOCompletionOverlapped::s_Refs;

	IOCompletionOverlapped::IOCompletionOverlapped()
		: OverlappedBuffer{}
	{
		PlatformAtomics::InterlockedIncrement(&s_Refs);
		SelfPtr(OverlappedBuffer) = this;
	}

	IOCompletionOverlapped::~IOCompletionOverlapped() noexcept
	{
		PlatformAtomics::InterlockedDecrement(&s_Refs);
	}

	void IOCompletionOverlapped::Assert()
	{
		check(s_Refs == 0);
	}
}