// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Net/Uri.h"
#include "Http/HttpVerbs.h"

class NETWORK_API HttpRequest
{
	Uri _baseUri;
	EHttpVerbs _verbs = EHttpVerbs::GET;
	std::vector<std::pair<String, String>> _queries;
	std::map<String, String> _headers;

public:
	HttpRequest();

	HttpRequest& SetUrl(const Uri& uri) & noexcept;
	inline HttpRequest&& SetUrl(const Uri& uri) && noexcept { return (HttpRequest&&)SetUrl(uri); };

	HttpRequest& SetVerbs(const EHttpVerbs& verbs) & noexcept;
	inline HttpRequest&& SetVerbs(const EHttpVerbs& verbs) && noexcept { return (HttpRequest&&)SetVerbs(verbs); };

	HttpRequest& SetQuery(const String& name, const String& value) & noexcept;
	inline HttpRequest&& SetQuery(const String& name, const String& value) && noexcept { return (HttpRequest&&)SetQuery(name, value); };

	HttpRequest& SetHeader(const String& name, const String& value) & noexcept;
	inline HttpRequest&& SetHeader(const String& name, const String& value) && noexcept { return (HttpRequest&&)SetHeader(name, value); }

	String Compose();
};