// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Diagnostics/StackFrame.h"
#include "Threading/Thread.h"
#include <span>
#include <vector>

namespace Ayla
{
	class CORE_API StackTrace
	{
	private:
		std::vector<StackFrame> Frames;

	public:
		StackTrace() noexcept;

		std::span<const StackFrame> GetFrames() const noexcept { return Frames; }
		size_t GetFrameCount() const noexcept { return Frames.size(); }
		String ToString() const;

	public:
		static StackTrace Current() noexcept;
		static StackTrace FromThread(const Thread& Thrd) noexcept;
		static StackTrace FromException(void* lpExceptionPointers) noexcept;
	};
}