// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Http/HttpClient.h"
//#include "Net/Socket.h"
//#include "Http/HttpRequest.h"
//#include "HttpClient.gen.cpp"
//
//HttpClient::HttpClient()
//{
//}
//
//HttpClient::~HttpClient() noexcept
//{
//}
//
//Task<HttpResponse> HttpClient::SendAsync(HttpRequest request, std::stop_token cancellationToken)
//{
//	Uri uri = request.GetUri();
//	String host = uri.GetHost();
//
//	IPAddress ip;
//	if (IPAddress::TryParse(host, ip) == false)
//	{
//		throw ArgumentException(TEXT("Cannot resolve hostname."));
//	}
//
//	Socket sock = Socket::CreateTCP();
//	IPEndPoint ep = { .Address = ip, .Port = (uint16)uri.GetPort() };
//
//	// Connect to endpoint.
//	co_await sock.ConnectAsync(ep);
//
//	// Sending request.
//	{
//		std::string buf = request.Compose();
//		size_t send = 0;
//		while (send < buf.length())
//		{
//			std::span<const char> subspan = std::span<char const>(buf.c_str() + send, buf.length() - send);
//			send += co_await sock.SendAsync(subspan, cancellationToken);
//		}
//	}
//
//	HttpResponse response;
//	std::vector<char> buf(1024);
//	String bufstr;
//	auto handle = response.AppendBuffer(&bufstr);
//	auto it = handle.begin();
//
//	while (it.IsDone() == false)
//	{
//		size_t recv = co_await sock.ReceiveAsync(buf, cancellationToken);
//		if (recv == 0)
//		{
//			co_return response;
//		}
//
//		bufstr += String::FromCodepage(std::string_view(buf.data(), recv), 65001);
//		if (*(++it))
//		{
//			break;
//		}
//	}
//
//	sock.Close();
//	co_return response;
//}
//
//Task<HttpResponse> HttpClient::GetAsync(const Uri& uri, std::stop_token cancellationToken)
//{
//	return SendAsync(HttpRequest().SetUri(uri).SetVerbs(EHttpVerbs::GET), cancellationToken);
//}
//
//Task<HttpResponse> HttpClient::PostAsync(const Uri& uri, const String& body, std::stop_token cancellationToken)
//{
//	return SendAsync(HttpRequest().SetUri(uri).SetVerbs(EHttpVerbs::POST).SetBody(body), cancellationToken);
//}
//
//HttpClient& HttpClient::GetSingleton()
//{
//	static HttpClient sClient;
//	return sClient;
//}