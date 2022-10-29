// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Net/Uri.h"
#include "Net/IPEndPoint.h"
#include "Http/HttpRequest.h"
#include "Http/HttpResponse.h"
#include "Net/Socket.h"
#include "HttpClient.gen.h"

SCLASS()
class NETWORK_API HttpClient : implements Object
{
	GENERATED_BODY()

private:
	Spinlock _lock;
	std::vector<Socket> _socketPool;

private:
	HttpClient();
	virtual ~HttpClient() noexcept override;

public:
	Task<HttpResponse> SendAsync(HttpRequest request, std::stop_token cancellationToken = {});
	Task<HttpResponse> GetAsync(const Uri& uri, std::stop_token cancellationToken = {});
	Task<HttpResponse> PostAsync(const Uri& uri, const String& body, std::stop_token cancellationToken = {});

	static HttpClient& GetSingleton();
};