// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Net/Uri.h"
#include "Http/HttpVerbs.h"
#include "Http/HttpResponse.h"

class NETWORK_API HttpRequest
{
	Uri _baseUri;
	EHttpVerbs _verbs = EHttpVerbs::GET;
	std::vector<std::pair<String, String>> _queries;
	std::map<String, String> _headers;
	String _body;

public:
	HttpRequest();

	HttpRequest& SetUri(const Uri& uri) & noexcept;
	inline HttpRequest&& SetUri(const Uri& uri) && noexcept { return (HttpRequest&&)SetUri(uri); };
	const Uri& GetUri() const noexcept { return _baseUri; }

	HttpRequest& SetVerbs(const EHttpVerbs& verbs) & noexcept;
	inline HttpRequest&& SetVerbs(const EHttpVerbs& verbs) && noexcept { return (HttpRequest&&)SetVerbs(verbs); };

	HttpRequest& SetQuery(const String& name, const String& value) & noexcept;
	inline HttpRequest&& SetQuery(const String& name, const String& value) && noexcept { return (HttpRequest&&)SetQuery(name, value); };

	HttpRequest& SetHeader(const String& name, const String& value) & noexcept;
	inline HttpRequest&& SetHeader(const String& name, const String& value) && noexcept { return (HttpRequest&&)SetHeader(name, value); }

	HttpRequest& SetBody(const String& body) & noexcept;
	inline HttpRequest&& SetBody(const String& body) && noexcept { return (HttpRequest&&)SetBody(body); }

	std::string Compose();

	// Helper on HttpClient
	Task<HttpResponse> SendAsync();
};