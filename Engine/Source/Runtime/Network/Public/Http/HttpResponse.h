// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Http/HttpResponseCode.h"

class NETWORK_API HttpResponse
{
	friend class HttpClient;

private:
	EHttpResponseCode _code = EHttpResponseCode::Unknown;
	std::map<String, String> _headers;
	String _body;

public:
	HttpResponse() noexcept;
	~HttpResponse() noexcept;

	inline EHttpResponseCode ResponseCode() const noexcept { return _code; }
	std::optional<String> GetHeader(const String& name) const noexcept;
	inline String GetBody() const noexcept { return _body; }

	inline bool Ok() const noexcept { return (int32)_code >= 200 && (int32)_code < 300; }

private:
	Linq::Generator<bool> AppendBuffer(String* dynamicBuf);
};