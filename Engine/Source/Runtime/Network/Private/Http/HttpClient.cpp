// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Http/HttpClient.h"
#include "Net/Socket.h"
#include "Http/HttpRequest.h"
#include "HttpClient.gen.cpp"

HttpClient::HttpClient()
{
}

HttpClient::~HttpClient() noexcept
{
}

Task<String> HttpClient::GetAsync(const Uri& uri, std::stop_token cancellationToken)
{
	String host = uri.GetHost();

	IPAddress ip;
	IPAddress::TryParse(host, ip);

	auto sock = Socket::CreateTCP();
	co_await sock.ConnectAsync(IPEndPoint{ .Address = ip, .Port = (uint16)uri.GetPort() });

	std::string buf = HttpRequest()
		.SetUrl(uri)
		.SetVerbs(EHttpVerbs::GET)
		.Compose()
		.AsCodepage(65001);
	size_t send = 0;
	while (send < buf.length())
	{
		send += co_await sock.SendAsync(buf, cancellationToken);
	}

	std::vector<char> response;
	response.resize(16384);
	size_t recv = co_await sock.ReceiveAsync(response, cancellationToken);

	co_return String::FromCodepage(std::string_view(response.data(), recv), 65001);
}