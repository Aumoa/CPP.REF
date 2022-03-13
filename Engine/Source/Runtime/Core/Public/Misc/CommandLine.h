// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "CoreConcepts.h"
#include <string_view>
#include <span>
#include <map>

class CommandLine
{
private:
	std::wstring _source;
	std::map<std::wstring, std::vector<std::wstring>, std::less<>> _keyValuePairs;

public:
	template<class TChar>
	CommandLine(size_t argc, TChar* const* argv)
	{
		std::vector<std::basic_string_view<TChar>> args(argc);
		for (size_t i = 0; i < args.size(); ++i)
		{
			args[i] = argv[i];
		}

		DoParse(args);
	}

	template<class TList>
	CommandLine(const TList& args) requires
		IString<EnumerableItem_t<TList>, StringChar_t<EnumerableItem_t<TList>>>
	{
		using String_t = EnumerableItem_t<TList>;
		using Char_t = StringChar_t<String_t>;

		if constexpr (std::same_as<EnumerableItem_t<TList>, std::basic_string_view<Char_t>>)
		{
			// Is span convertible. Just pass.
			DoParse(args);
		}
		else
		{
			std::vector<std::basic_string_view<Char_t>> args_sv(args.size());
			for (size_t i = 0; i < args.size(); ++i)
			{
				args_sv[i] = std::basic_string_view<Char_t>(args[i]);
			}
			DoParse(args_sv);
		}
	}

	bool ContainsKey(std::wstring_view key) const
	{
		return _keyValuePairs.contains(key);
	}

	bool TryGetValue(std::wstring_view key, std::span<const std::wstring>& outValues) const
	{
		if (auto it = _keyValuePairs.find(key); it != _keyValuePairs.end())
		{
			outValues = it->second;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool TryGetValue(std::wstring_view key, std::wstring& outValues) const
	{
		if (std::span<const std::wstring> values; TryGetValue(key, values) && values.size() > 0)
		{
			outValues = values[0];
			return true;
		}
		else
		{
			return false;
		}
	}

private:
	template<class TChar>
	void DoParse(std::span<const std::basic_string_view<TChar>> args)
	{
		using TNakedChar = std::remove_const_t<TChar>;

		static constexpr bool bWstr = std::same_as<TNakedChar, wchar_t>;
		using StringView_t = std::basic_string_view<TNakedChar>;

		StringView_t currKey;
		std::vector<std::wstring> values;

		char stringCtxKey = 0;
		std::wstring stringCtxAppend;

		StringView_t doublehyphen;
		if constexpr (bWstr)
		{
			doublehyphen = L"--";
		}
		else
		{
			doublehyphen = "--";
		}

		auto flushAll = [&]()
		{
			std::vector<std::wstring> clone;
			std::swap(clone, values);

			if constexpr (bWstr)
			{
				auto emplaced = _keyValuePairs.emplace(currKey, std::move(clone));
				if (!emplaced.second)
				{
					throw commandline_parser_error(std::format(L"Duplicated command line key({}).", currKey));
				}
			}
			else
			{
				auto wCurrKey = Cast<std::wstring>(currKey);
				auto emplaced = _keyValuePairs.emplace(wCurrKey, std::move(clone));
				if (!emplaced.second)
				{
					throw commandline_parser_error(std::format(L"Duplicated command line key({}).", wCurrKey));
				}
			}
		};

		for (size_t i = 0; i < args.size(); ++i)
		{
			auto sView = StringView_t(args[i]);

			if (stringCtxKey == 0 && sView.starts_with(doublehyphen))
			{
				if (!currKey.empty())
				{
					flushAll();
				}

				currKey = sView.substr(2);
				stringCtxAppend = L"";
			}
			else
			{
				if (stringCtxKey == 0)
				{
					if (sView.length() > 0 && (sView[0] == '\"' || sView[0] == '\'' || sView[0] == '`'))
					{
						stringCtxKey = (char)sView[0];
						sView = sView.substr(1);
					}
					else
					{
						if constexpr (bWstr)
						{
							values.emplace_back(sView);
						}
						else
						{
							values.emplace_back(ANSI_TO_WCHAR(sView));
						}
					}
				}

				if (stringCtxKey != 0)
				{
					size_t last = sView.length() - 1;
					if (last != -1 && (sView[last] == stringCtxKey))
					{
						stringCtxKey = 0;
						sView = sView.substr(0, sView.length() - 1);
					}

					if constexpr (bWstr)
					{
						stringCtxAppend += sView;
					}
					else
					{
						stringCtxAppend += Cast<std::wstring>(sView);
					}

					if (stringCtxKey == 0)
					{
						std::wstring clone;
						std::swap(clone, stringCtxAppend);
						values.emplace_back(std::move(clone));
					}
				}
			}
		}

		flushAll();
	}
};