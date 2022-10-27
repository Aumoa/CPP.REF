// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Http/HttpRequest.h"

HttpRequest::HttpRequest()
{
}

HttpRequest& HttpRequest::SetUrl(const Uri& uri) & noexcept
{
	_baseUri = uri;
	return *this;
}

HttpRequest& HttpRequest::SetVerbs(const EHttpVerbs& verbs) & noexcept
{
	_verbs = verbs;
	return *this;
}

HttpRequest& HttpRequest::SetQuery(const String& name, const String& value) & noexcept
{
	_queries.emplace_back(name, value);
	return *this;
}

HttpRequest& HttpRequest::SetHeader(const String& name, const String& value) & noexcept
{
	_headers[name] = value;
	return *this;
}

String HttpRequest::Compose()
{
	if (auto it = _headers.find(TEXT("Accept")); it == _headers.end())
	{
		_headers[TEXT("Accept")] = TEXT("*/*");
	}

	String format =
		TEXT("{} {} HTTP/1.1\r\n")
		TEXT("Host: {}:{}\r\n")
		TEXT("User-Agent: cppref/network\r\n")
		TEXT("{}\r\n")
		TEXT("\r\n");

	std::vector<String> headers;
	for (auto& [name, value] : _headers)
	{
		headers.emplace_back(name + TEXT(": ") + value);
	}

	static constexpr String verbStr[4] =
	{
		TEXT("GET"),
		TEXT("POST"),
		TEXT("PUT"),
		TEXT("DELETE")
	};

	return String::Format(format,
		verbStr[(size_t)_verbs], _baseUri.GetPath(),
		_baseUri.GetHost(), _baseUri.GetPort(),
		String::Join(TEXT("\r\n"), headers)
	);
}