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

namespace libty::inline Core
{
	struct LogEntry
	{
		std::thread::id ThreadId;
		String ThreadName;
		DateTime LoggedTime;
		LogCategory* Category;
		ELogVerbosity Verbosity;

		String Message;
		std::source_location Source;

		LogEntry&& Generate() &&
		{
			_log = String::Format(TEXT("{}: {}: {}"),
				Category->GetName(),
				Category->VerbosityToString(Verbosity),
				Message
			);

			_details = String::Format(TEXT("{}: {}: {}\n  at {} in {}:{}"),
				LoggedTime.ToString<libty::DateTimeFormat::Json>(),
				ThreadName,
				_log,
				String::FromLiteral(Source.function_name()),
				std::filesystem::path(Source.file_name()).filename().wstring(),
				Source.line()
			);

			return std::move(*this);
		}

	private:
		String _log;
		String _details;

	public:
		String ToString(bool details = false)
		{
			return details ? _details : _log;
		}
	};
}