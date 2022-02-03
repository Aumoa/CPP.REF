// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Misc/CommandLine.h"
#include "Misc/StringUtils.h"
#include "LogCore.h"
#include "Exceptions/Exception.h"

class SCommandLineParserException : public SException
{
	GENERATED_BODY(SCommandLineParserException)

public:
	SCommandLineParserException(std::wstring_view Message) : Super(Message)
	{
	}
};

GENERATE_BODY(SCommandLineParserException);

CommandLine::CommandLine(int32 Argc, char* const* Argv)
{
	Parse(Argc, Argv);
}

CommandLine::CommandLine(int32 Argc, wchar_t* const* Argv)
{
	Parse(Argc, Argv);
}

CommandLine::CommandLine(std::span<const std::wstring> Args)
{
	std::vector<const wchar_t*> Buffers;
	Buffers.reserve(Args.size());

	for (auto& Item : Args)
	{
		Buffers.emplace_back(Item.c_str());
	}

	Parse((int32)Args.size(), Buffers.data());
}

bool CommandLine::ContainsKey(std::wstring_view Key) const
{
	return KeyValuePairs.contains(Key);
}

bool CommandLine::TryGetValue(std::wstring_view Key, std::span<const std::wstring>& OutValues) const
{
	if (auto It = KeyValuePairs.find(Key); It != KeyValuePairs.end())
	{
		OutValues = It->second;
		return true;
	}
	else
	{
		return false;
	}
}

template<class TChar>
void CommandLine::Parse(int32 Argc, TChar* const* Args)
{
	using TNakedChar = std::remove_const_t<TChar>;

	static constexpr bool bWstr = std::same_as<TNakedChar, wchar_t>;
	using StringView_t = std::basic_string_view<TNakedChar>;

	StringView_t CurrKey;
	std::vector<std::wstring> Values;

	char StringCtxKey = 0;
	std::wstring StringCtxAppend;

	StringView_t Doublehyphen;
	if constexpr (bWstr)
	{
		Doublehyphen = L"--";
	}
	else
	{
		Doublehyphen = "--";
	}

	auto FlushAll = [&]()
	{
		std::vector<std::wstring> Clone;
		std::swap(Clone, Values);

		if constexpr (bWstr)
		{
			auto Emplaced = KeyValuePairs.emplace(CurrKey, std::move(Clone));
			if (!Emplaced.second)
			{
				throw gcnew SCommandLineParserException(std::format(L"Duplicated command line key({}).", CurrKey));
			}
		}
		else
		{
			auto WCurrKey = ANSI_TO_WCHAR(CurrKey);
			auto Emplaced = KeyValuePairs.emplace(WCurrKey, std::move(Clone));
			if (!Emplaced.second)
			{
				throw gcnew SCommandLineParserException(std::format(L"Duplicated command line key({}).", WCurrKey));
			}
		}
	};

	for (int32 i = 0; i < Argc; ++i)
	{
		auto Sview = StringView_t(Args[i]);

		if (StringCtxKey == 0 && Sview.starts_with(Doublehyphen))
		{
			if (!CurrKey.empty())
			{
				FlushAll();
			}

			CurrKey = Sview.substr(2);
			StringCtxAppend = L"";
		}
		else
		{
			if (StringCtxKey == 0)
			{
				if (Sview.length() > 0 && (Sview[0] == '\"' || Sview[0] == '\'' || Sview[0] == '`'))
				{
					StringCtxKey = (char)Sview[0];
					Sview = Sview.substr(1);
				}
				else
				{
					if constexpr (bWstr)
					{
						Values.emplace_back(Sview);
					}
					else
					{
						Values.emplace_back(ANSI_TO_WCHAR(Sview));
					}
				}
			}

			if (StringCtxKey != 0)
			{
				size_t Last = Sview.length() - 1;
				if (Last != -1 && (Sview[Last] == StringCtxKey))
				{
					StringCtxKey = 0;
					Sview = Sview.substr(0, Sview.length() - 1);
				}

				if constexpr (bWstr)
				{
					StringCtxAppend += Sview;
				}
				else
				{
					StringCtxAppend += ANSI_TO_WCHAR(Sview);
				}

				if (StringCtxKey == 0)
				{
					std::wstring Clone;
					std::swap(Clone, StringCtxAppend);
					Values.emplace_back(std::move(Clone));
				}
			}
		}
	}

	FlushAll();
}