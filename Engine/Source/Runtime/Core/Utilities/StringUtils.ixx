// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Core:StringUtils;

import std.core;
import :PrimitiveTypes;

/// <summary>
/// Provide extended functions for compose string.
/// </summary>
export class StringUtils abstract final
{
public:
	/// <summary>
	/// Join all string-convertible arguments with separator.
	/// </summary>
	/// <typeparam name="T"> Type of string convertible that implements std::formatter. </typeparam>
	/// <param name="separator"> The separator. </param>
	/// <param name="arguments"> The arguments. </param>
	/// <returns> The composed string. </returns>
	template<class T>
	static std::wstring Join(std::wstring_view separator, std::span<T const> arguments)
	{
		if (arguments.size() == 0)
		{
			return L"";
		}

		std::wstringstream wss;
		wss << std::format(L"{}", arguments[0]);
		for (size_t i = 1; i < arguments.size(); ++i)
		{
			wss << separator << std::format(L"{}", arguments[i]);
		}
		return std::move(wss).str();
	}

	/// <summary>
	/// Get placeholder text for std::format.
	/// </summary>
	/// <param name="number"> The argument number. </param>
	/// <param name="formatArgs"> The formatting arguments includes colon. </param>
	/// <returns> The placeholder string. </returns>
	static std::wstring GetPlaceholder(int32 number, std::wstring_view formatArgs)
	{
		return std::format(L"{{{}{}}}", number, formatArgs);
	}

	/// <summary>
	/// Get placeholder text for std::format.
	/// </summary>
	/// <param name="formatArgs"> The formatting arguments includes colon. </param>
	/// <returns> The placeholder string. </returns>
	static std::wstring GetPlaceholder(std::wstring_view formatArgs)
	{
		return std::format(L"{{{}}}", formatArgs);
	}
};