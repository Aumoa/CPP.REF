// Copyright 2020-2024 Aumoa.lib. All right reserved.

#include "System/AssertionMacros.h"

import Core;

std::vector<String> CommandLine::Args;

void CommandLine::Init(int Argc, const char* const Argv[])
{
	check(Args.empty());
	for (int i = 0; i < Argc; ++i)
	{
		Args.emplace_back(String::FromLiteral(Argv[i]));
	}
}

void CommandLine::Init(String CmdArgs)
{
	check(Args.empty());
	bool bQuoted = false;

	if (CmdArgs.IsEmpty())
	{
		return;
	}

	auto FlushCommand = [&](size_t Start, size_t End)
	{
		check(Start < End);
		size_t Length = End - Start;
		if (Length == 1 && Char::IsWhiteSpace(CmdArgs[Start]))
		{
			return End;
		}

		Args.emplace_back(CmdArgs.Substring(Start, Length));
		return End;
	};

	size_t StartIndex = 0;
	for (size_t i = 0; i < CmdArgs.length(); ++i)
	{
		if (bQuoted)
		{
			if (CmdArgs[i] == '"' && CmdArgs[i - 1] != '\\')
			{
				bQuoted = false;
			}
		}

		if (Char::IsWhiteSpace(CmdArgs[i]))
		{
			StartIndex = FlushCommand(StartIndex, i);
		}
	}
	FlushCommand(StartIndex, CmdArgs.length());
}

bool CommandLine::TryGetValue(String InName, String& OutValue)
{
	bool bCatchResult = false;
	OutValue = String::GetEmpty();

	for (auto& Cur : Args)
	{
		if (Cur.StartsWith(TEXT("-")))
		{
			if (bCatchResult)
			{
				// Command is not contains.
				break;
			}

			String Command = Cur.Substring(1);
			if (Command.Equals(InName, EStringComparison::CurrentCultureIgnoreCase))
			{
				bCatchResult = true;
				continue;
			}
		}
		else if (bCatchResult)
		{
			OutValue = Cur.Trim();
			break;
		}
	}

	return bCatchResult;
}

bool CommandLine::Contains(String InName)
{
	String _;
	return TryGetValue(InName, _);
}