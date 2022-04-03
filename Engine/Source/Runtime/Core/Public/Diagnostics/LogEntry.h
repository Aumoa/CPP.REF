// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "Misc/DateTime.h"
#include "Misc/String.h"
#include "LogVerbosity.h"
#include "LogCategory.h"
#include <source_location>
#include <string>
#include <filesystem>
#include <thread>

namespace libty::inline Core::inline Diagnostics
{
	struct LogEntry
	{
		std::thread::id ThreadId;
		std::wstring ThreadName;
		DateTime LoggedTime;
		LogCategory* Category;
		ELogVerbosity Verbosity;

		std::wstring Message;
		std::source_location Source;

		LogEntry&& Generate() &&
		{
			_log = String::Format(L"{}: {}: {}",
				Category->GetName(),
				Category->VerbosityToString(Verbosity),
				Message
			);

			_details = String::Format(L"{}: {}: {}\n  at {} in {}:{}",
				LoggedTime.ToString<libty::DateTimeFormat::Json>(),
				ThreadName,
				_log,
				String::AsUnicode(Source.function_name()),
				std::filesystem::path(Source.file_name()).filename().wstring(),
				Source.line()
			);

			return std::move(*this);
		}

	private:
		std::wstring _log;
		std::wstring _details;

	public:
		std::wstring_view ToString(bool details = false)
		{
			return details ? _details : _log;
		}
	};
}