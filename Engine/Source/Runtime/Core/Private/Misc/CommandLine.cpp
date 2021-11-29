// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Misc/CommandLine.h"
#include "Diagnostics/LogCategory.h"
#include "Diagnostics/LogSystem.h"
#include "Diagnostics/LogVerbosity.h"
#include "LogCore.h"

size_t SCommandLine::GetArgument(std::wstring_view start, std::wstring* optional_tail) const
{
	for (size_t i = 0; i < ResolvedArgs.size(); ++i)
	{
		if (ResolvedArgs[i].starts_with(start))
		{
			if (optional_tail)
			{
				size_t tail_start = start.length();
				size_t tail_length = ResolvedArgs[i].length() - tail_start;

				*optional_tail = ResolvedArgs[i].substr(tail_start, tail_length);
			}

			return i;
		}
	}

	return -1;
}

std::optional<std::wstring_view> SCommandLine::GetArgument(size_t indexOf) const
{
	if (ResolvedArgs.size() <= indexOf)
	{
		SE_LOG(LogCore, Error, L"Index({}) is not valid.", indexOf);
		return std::nullopt;
	}

	return ResolvedArgs[indexOf];
}

const std::vector<std::wstring>& SCommandLine::GetArguments() const
{
	return ResolvedArgs;
}