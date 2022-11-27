// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Http/HttpClient.h"
#include "Net/Socket.h"
#include "Http/HttpRequest.h"

HttpClient::HttpClient()
{
}

HttpClient::~HttpClient() noexcept
{
}

Task<HttpResponse> HttpClient::SendAsync(HttpRequest request, std::stop_token cancellationToken)
{
	Uri uri = request.GetUri();
	String host = uri.GetHost();

	std::vector<IPAddress> addresses(1);
	if (IPAddress::TryParse(host, &addresses[0]) == false)
	{
		addresses.resize(0);
		for (auto& entry : co_await Dns::GetHostEntryAsync(host, cancellationToken))
		{
			addresses.insert_range(addresses.end(), entry.GetAddresses());
		}

		if (addresses.size() == 0)
		{
			throw ArgumentException(TEXT("Cannot resolve hostname."));
		}
	}

	Socket sock(EAddressFamily::InterNetwork, ESocketType::Stream, EProtocolType::Tcp);
	bool connected = false;
	std::vector<std::exception_ptr> exceptions;
	for (auto& address : addresses)
	{
		try
		{
			IPEndPoint ep(address, (uint16)uri.GetPort());
			co_await sock.ConnectAsync(ep);
			connected = true;
			break;
		}
		catch (const SocketException&)
		{
			exceptions.emplace_back(std::current_exception());
		}
	}

	// Connect to endpoint.
	if (connected == false)
	{
		std::rethrow_exception(exceptions.back());
	}

	// Sending request.
	{
		std::string buf = request.Compose();
		size_t send = 0;
		while (send < buf.length())
		{
			std::span<const char> subspan = std::span<char const>(buf.c_str() + send, buf.length() - send);
			send += co_await sock.SendAsync(subspan, cancellationToken);
		}
	}

	HttpResponse response;
	std::vector<char> buf(1024);
	String bufstr;
	auto handle = response.AppendBuffer(&bufstr);
	auto it = handle.begin();

	while (it.IsDone() == false)
	{
		size_t recv = co_await sock.ReceiveAsync(std::span(buf), cancellationToken);
		if (recv == 0)
		{
			co_return response;
		}

		bufstr += String::FromCodepage(std::string_view(buf.data(), recv), 65001);
		if (*(++it))
		{
			break;
		}
	}

	sock.Close();
	co_return response;
}

Task<HttpResponse> HttpClient::GetAsync(const Uri& uri, std::stop_token cancellationToken)
{
	return SendAsync(HttpRequest().SetUri(uri).SetVerbs(EHttpVerbs::GET), cancellationToken);
}

Task<HttpResponse> HttpClient::PostAsync(const Uri& uri, const String& body, std::stop_token cancellationToken)
{
	return SendAsync(HttpRequest().SetUri(uri).SetVerbs(EHttpVerbs::POST).SetBody(body), cancellationToken);
}

HttpClient& HttpClient::GetSingleton()
{
	static HttpClient sClient;
	return sClient;
}