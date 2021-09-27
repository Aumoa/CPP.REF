// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Misc/CommandLine.h"
#include "Diagnostics/LogCategory.h"
#include "Diagnostics/LogSystem.h"
#include "Diagnostics/LogVerbosity.h"
#include "LogCore.h"

size_t SCommandLine::GetArgument(std::wstring_view start, std::wstring* optional_tail) const
{
	for (size_t i = 0; i < _resolvedArgs.size(); ++i)
	{
		if (_resolvedArgs[i].starts_with(start))
		{
			if (optional_tail)
			{
				size_t tail_start = start.length();
				size_t tail_length = _resolvedArgs[i].length() - tail_start;

				*optional_tail = _resolvedArgs[i].substr(tail_start, tail_length);
			}

			return i;
		}
	}

	return -1;
}

std::optional<std::wstring_view> SCommandLine::GetArgument(size_t indexOf) const
{
	if (_resolvedArgs.size() <= indexOf)
	{
		SE_LOG(LogCore, Error, L"Index({}) is not valid.", indexOf);
		return std::nullopt;
	}

	return _resolvedArgs[indexOf];
}