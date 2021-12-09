// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include <string_view>
#include <vector>

class CORE_API SCommandLine : implements SObject
{
	GENERATED_BODY(SCommandLine)

private:
	std::vector<std::wstring> ResolvedArgs;

private:
	SCommandLine();

public:
	template<class T>
	SCommandLine(const T& InPlatformArgs) : SCommandLine()
	{
		std::optional<std::wostringstream> woss;

		ResolvedArgs.reserve(InPlatformArgs.size());
		for (auto& Arg : InPlatformArgs)
		{
			if (!woss)
			{
				if (Arg[0] == L'\"')
				{
					if (Arg.back() == L'\"')
					{
						ResolvedArgs.emplace_back(Arg.substr(1, Arg.length() - 2));
					}
					else
					{
						woss.emplace(std::wstring(Arg.substr(1)));
					}
				}
				else
				{
					ResolvedArgs.emplace_back(Arg);
				}
			}
			else
			{
				if (Arg.back() == L'\"')
				{
					*woss << Arg.substr(0, Arg.length() - 1);
					ResolvedArgs.emplace_back(woss->str());
					woss.reset();
				}
				else
				{
					*woss << Arg;
				}
			}
		}
	}

	size_t GetArgument(std::wstring_view start, std::wstring* optional_tail = nullptr) const;
	std::optional<std::wstring_view> GetArgument(size_t indexOf) const;
	const std::vector<std::wstring>& GetArguments() const;
};