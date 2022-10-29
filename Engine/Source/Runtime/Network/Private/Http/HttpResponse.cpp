// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Http/HttpResponse.h"

HttpResponse::HttpResponse() noexcept
{
}

HttpResponse::~HttpResponse() noexcept
{
}

std::optional<String> HttpResponse::GetHeader(const String& name) const noexcept
{
	auto it = _headers.find(name);
	if (it == _headers.end())
	{
		return std::nullopt;
	}

	return it->second;
}

Linq::Generator<bool> HttpResponse::AppendBuffer(String* dynamicBuf)
{
	constexpr bool Failure = true;
	constexpr bool KeepRunning = false;
	String& buf = *dynamicBuf;

	// Parse HTTP version.
	{
		constexpr String HTTP_1_1 = TEXT("http/1.1");

		while (buf.length() < HTTP_1_1.length())
		{
			for (size_t i = 0; i < std::min(buf.length(), HTTP_1_1.length()); ++i)
			{
				if (HTTP_1_1[i] != buf[i])
				{
					co_yield Failure;
				}
			}

			co_yield KeepRunning;
		}

		buf = buf.Substring(HTTP_1_1.length());
	}

	// Parse HTTP response code.
	{
		size_t indexOf = 0;
		while ((indexOf = buf.IndexOf(TEXT("\r\n"))) == -1)
		{
			co_yield KeepRunning;
		}

		String substr = buf.Substring(0, indexOf);
		buf = buf.Substring(indexOf + 2);

		std::vector<String> splits = substr.Split(' ', EStringSplitOptions::TrimEntries | EStringSplitOptions::RemoveEmptyEntries);
		if (splits.size() < 2)
		{
			co_yield Failure;
		}

		_code = (EHttpResponseCode)String::Stoi(splits[0]);
	}

	size_t content_length = 0;
	bool chunked = false;

	// Parse headers.
	while (true)
	{
		size_t indexOf = 0;
		while ((indexOf = buf.IndexOf(TEXT("\r\n"))) == -1)
		{
			co_yield KeepRunning;
		}

		String line = buf.Substring(0, indexOf);
		buf = buf.Substring(indexOf + 2);

		if (line.IsEmpty())
		{
			break;
		}

		size_t separator = line.IndexOf(':');
		if (separator == -1)
		{
			co_yield Failure;
		}

		String name = line.Substring(0, separator);
		String value = line.Substring(separator + 1).TrimStart();
		_headers.emplace(name, value);

		if (content_length > 0 || chunked == true)
		{
			continue;
		}

		if (name == TEXT("Content-Length"))
		{
			content_length = String::Stoi(value);
		}
		else if (name == TEXT("Transfer-Encoding") && value == TEXT("chunked"))
		{
			chunked = true;
		}
	}

	if (content_length > 0)
	{
		while (buf.length() < content_length)
		{
			co_yield KeepRunning;
		}

		_body = buf.Substring(0, content_length);
		buf = buf.Substring(content_length);
	}
	else if (chunked)
	{
		while (true)
		{
			size_t indexOf = 0;
			while ((indexOf = buf.IndexOf(TEXT("\r\n"))) == -1)
			{
				co_yield KeepRunning;
			}

			String line = buf.Substring(0, indexOf);
			buf = buf.Substring(indexOf + 2);

			size_t chunked_size = String::Stoi(line, true);
			if (chunked_size == 0)
			{
				break;
			}

			chunked_size += 2;
			while (buf.length() < chunked_size)
			{
				co_yield KeepRunning;
			}

			_body += buf.Substring(0, chunked_size - 2);
			buf = buf.Substring(chunked_size);
		}
	}
}