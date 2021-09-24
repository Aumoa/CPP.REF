// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include <string_view>
#include <vector>

class SCommandLine : implements SObject
{
	GENERATED_BODY(SCommandLine)

private:
	std::vector<std::wstring> _resolvedArgs;

public:
	template<class TString>
	SCommandLine(const std::vector<TString>& platformArgs)
	{
		std::optional<std::wostringstream> woss;

		_resolvedArgs.reserve(platformArgs.size());
		for (size_t i = 0; i < platformArgs.size(); ++i)
		{
			const std::wstring_view& ctx = platformArgs[i];

			if (!woss)
			{
				if (ctx[0] == L'\"')
				{
					if (ctx.back() == L'\"')
					{
						_resolvedArgs.emplace_back(ctx.substr(1, ctx.length() - 2));
					}
					else
					{
						woss.emplace(std::wstring(ctx.substr(1)));
					}
				}
				else
				{
					_resolvedArgs.emplace_back(ctx);
				}
			}
			else
			{
				if (ctx.back() == L'\"')
				{
					*woss << ctx.substr(0, ctx.length() - 1);
					_resolvedArgs.emplace_back(woss->str());
					woss.reset();
				}
				else
				{
					*woss << ctx;
				}
			}
		}
	}

	CORE_API size_t GetArgument(std::wstring_view start, std::wstring* optional_tail = nullptr) const;
	CORE_API std::optional<std::wstring_view> GetArgument(size_t indexOf) const;
};