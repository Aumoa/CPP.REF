// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Stackframe.h"
#include <thread>

class CORE_API Stacktrace
{
	struct _stacktrace_impl;

private:
	std::vector<Stackframe> _frames;

public:
	Stacktrace() noexcept;

	std::span<const Stackframe> GetStackframes() const noexcept;
	String ToString() const noexcept;

public:
	static Stacktrace Current() noexcept;
	static Stacktrace FromThread(std::thread& thr) noexcept;
	static Stacktrace FromException(void* lpExceptionPointers) noexcept;
};