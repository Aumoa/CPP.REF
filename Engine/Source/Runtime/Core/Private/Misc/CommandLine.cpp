// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Misc/CommandLine.h"

std::vector<String> CommandLine::Args;

void CommandLine::Set(int Argc, const char* Argv[])
{
	for (int i = 0; i < Argc; ++i)
	{
		Args.emplace_back(String::FromLiteral(Argv[i]));
	}
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
			OutValue = Cur;
			break;
		}
	}

	return bCatchResult;
}