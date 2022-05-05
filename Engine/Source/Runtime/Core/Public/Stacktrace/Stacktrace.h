// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "Stackframe.h"
#include "Linq/Generator.h"
#include "Misc/StringView.h"
#include <span>

namespace libty::inline Core
{
	class Thread;

	class CORE_API Stacktrace
	{
		struct _Stacktrace_impl;
		std::shared_ptr<_Stacktrace_impl> _impl;

	private:
		Stacktrace() noexcept;

	public:
		Stacktrace(const Stacktrace&) noexcept = default;
		Stacktrace(Stacktrace&&) noexcept = default;

		std::span<const Stackframe> GetStackframes() const noexcept;

	public:
		static Stacktrace CaptureCurrent() noexcept;
		static Stacktrace Capture(Thread* thread) noexcept;
		static Stacktrace CaptureException(void* exception_pointer) noexcept;
	};
}