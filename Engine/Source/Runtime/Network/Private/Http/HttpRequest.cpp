// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Http/HttpRequest.h"
#include "Http/HttpClient.h"

HttpRequest::HttpRequest()
{
}

HttpRequest& HttpRequest::SetUri(const Uri& uri) & noexcept
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

HttpRequest& HttpRequest::SetBody(const String& body) & noexcept
{
	_body = body;
	return *this;
}

std::string HttpRequest::Compose()
{
	if (auto it = _headers.find(TEXT("Accept")); it == _headers.end())
	{
		_headers[TEXT("Accept")] = TEXT("*/*");
	}

	std::string body;
	if (_body.length() > 0)
	{
		body = _body.AsCodepage(65001);
	}

	String format =
		TEXT("{} {} HTTP/1.1\r\n")
		TEXT("Host: {}:{}\r\n")
		TEXT("User-Agent: cppref/network\r\n")
		TEXT("Content-Length: {}\r\n")
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

	std::string head = String::Format(format,
		/*HTTP/1.1*/		verbStr[(size_t)_verbs], _baseUri.GetPath(),
		/*Host*/			_baseUri.GetHost(), _baseUri.GetPort(),
		/*Content-Length*/	body.length(),
		/*Headers*/			String::Join(TEXT("\r\n"), headers)
	).AsCodepage(65001);

	return head + body;
}

Task<HttpResponse> HttpRequest::SendAsync()
{
	return HttpClient::GetSingleton().SendAsync(*this);
}