// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Core:StringUtils;

import std.core;
import :PrimitiveTypes;

using namespace std;

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
	static wstring Join(wstring_view separator, span<T const> arguments)
	{
		if (arguments.size() == 0)
		{
			return L"";
		}

		wstringstream wss;
		wss << format(L"{}", arguments[0]);
		for (size_t i = 1; i < arguments.size(); ++i)
		{
			wss << separator << format(L"{}", arguments[i]);
		}
		return move(wss).str();
	}

	/// <summary>
	/// Get placeholder text for format.
	/// </summary>
	/// <param name="number"> The argument number. </param>
	/// <param name="formatArgs"> The formatting arguments includes colon. </param>
	/// <returns> The placeholder string. </returns>
	static wstring GetPlaceholder(int32 number, wstring_view formatArgs);

	/// <summary>
	/// Get placeholder text for format.
	/// </summary>
	/// <param name="formatArgs"> The formatting arguments includes colon. </param>
	/// <returns> The placeholder string. </returns>
	static wstring GetPlaceholder(wstring_view formatArgs);

	/// <summary>
	/// Convert multibyte string to unicode string.
	/// </summary>
	/// <param name="multibyte"> The multibyte string. </param>
	/// <param name="codePage"> The code page that desired converted. </param>
	/// <returns> Converted unicode string. </returns>
	static wstring AsUnicode(const string& multibyte, uint32 codePage = 0);

	/// <summary>
	/// Convert unicode string to multibyte string.
	/// </summary>
	/// <param name="unicode"> The unicode string. </param>
	/// <param name="codePage"> The code page that desired converted. </param>
	/// <returns> Converted multibyte string. </returns>
	static string AsMultibyte(const wstring& unicode, uint32 codePage = 0);
};