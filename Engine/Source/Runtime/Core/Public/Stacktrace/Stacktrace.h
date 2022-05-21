// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "Stackframe.h"
#include "Misc/String.h"
#include <span>

namespace libty::inline Core
{
	class Thread;

	class CORE_API Stacktrace
	{
		struct _Stacktrace_impl;
		std::shared_ptr<_Stacktrace_impl> _impl;

	public:
		Stacktrace() noexcept;
		Stacktrace(const Stacktrace&) noexcept = default;
		Stacktrace(Stacktrace&&) noexcept = default;

		std::span<const Stackframe> GetStackframes() const noexcept;

		[[nodiscard]] inline bool IsValid() const noexcept
		{
			return (bool)_impl;
		}

		inline String Trace() const noexcept
		{
			std::vector<String> frames;
			for (auto& Callstack : GetStackframes())
			{
				frames.emplace_back(String::Format(TEXT("   at {}!{} in {}{}"),
					Callstack.ModuleName,
					Callstack.GetCleanedFunctionName(),
					Callstack.GetCleanedFileName(),
					!Callstack.FileName ? TEXT("") : String::Format(TEXT("({})"), Callstack.Line)
				));
			}

			return String::Join(TEXT("\n"), frames);
		}

	public:
		static Stacktrace CaptureCurrent() noexcept;
		static Stacktrace Capture(Thread* thread) noexcept;
		static Stacktrace CaptureException(void* exception_pointer) noexcept;
	};
}