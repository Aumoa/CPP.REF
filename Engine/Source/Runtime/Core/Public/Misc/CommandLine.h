// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include <string_view>
#include <span>
#include <map>

class CORE_API CommandLine
{
private:
	std::wstring Source;
	std::map<std::wstring, std::vector<std::wstring>, std::less<>> KeyValuePairs;

public:
	CommandLine(int32 Argc, char* const* Argv);
	CommandLine(int32 Argc, wchar_t* const* Argv);
	CommandLine(std::span<const std::wstring> Args);

	bool ContainsKey(std::wstring_view Key) const;
	bool TryGetValue(std::wstring_view Key, std::span<const std::wstring>& OutValues) const;
	
public:
	bool TryGetValue(std::wstring_view Key, std::wstring& OutValue) const
	{
		if (std::span<const std::wstring> Values; TryGetValue(Key, Values) && Values.size() > 0)
		{
			OutValue = Values[0];
			return true;
		}
		else
		{
			return false;
		}
	}

private:
	template<class TChar>
	void Parse(int32 Argc, TChar* const* Args);
};