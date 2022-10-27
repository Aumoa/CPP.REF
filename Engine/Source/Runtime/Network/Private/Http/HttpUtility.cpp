// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Http/HttpUtility.h"
#include "Exceptions/ArgumentException.h"

String HttpUtility::UrlEncode(const String& url) noexcept
{
	static thread_local std::vector<char> buf;

	std::string utf8 = url.AsCodepage(65001);
	buf.resize(utf8.length() * 3);

	for (size_t i = 0; i < utf8.length(); ++i)
	{
		const unsigned char& ch = (const unsigned char&)utf8[i];
		EncodeChar(ch, buf.data() + 3 * i);
	}

	return String::FromLiteral(std::string_view(buf.data(), buf.size()));
}

String HttpUtility::UrlDecode(const String& encodedUrl)
{
	static thread_local std::vector<char> buf;
	buf.resize(0);
	buf.reserve(encodedUrl.length());

	for (size_t i = 0; i < encodedUrl.length(); ++i)
	{
		char_t ch = encodedUrl[i];
		if (ch == '%')
		{
			buf.emplace_back(DecodeChar(encodedUrl[i + 1], encodedUrl[i + 2]));
			i += 2;
		}
		else
		{
			if (IsUrlSafeChar(ch) == false)
			{
				throw ArgumentException(String::Format(TEXT("Character '{}' is not Url-safe."), ch));
			}

			buf.emplace_back((char)ch);
		}
	}

	return String::FromCodepage(std::string_view(buf.data(), buf.size()), 65001);
}

bool HttpUtility::IsUrlSafeChar(char_t ch) noexcept
{
	return isdigit(ch) || isalpha(ch)
		|| ch == 0x21
		|| ch == 0x27
		|| ch == 0x28
		|| ch == 0x29
		|| ch == 0x2A
		|| ch == 0x2B
		|| ch == 0x2C
		|| ch == 0x2D
		|| ch == 0x2E
		|| ch == 0x5F;
}

inline void HttpUtility::EncodeChar(unsigned char ch, char* buf) noexcept
{
	buf[0] = '%';
	String::Hex(ch, buf + 1);
}

inline char HttpUtility::DecodeChar(char_t ch1, char_t ch2) noexcept
{
	return (String::CharHex(ch1) << 4 | String::CharHex(ch2));
}