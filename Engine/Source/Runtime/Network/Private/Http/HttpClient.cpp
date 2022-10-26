// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Http/HttpClient.h"
#include "Net/Socket.h"
#include "HttpClient.gen.cpp"

HttpClient::HttpClient()
{
}

HttpClient::~HttpClient() noexcept
{
}

Task<String> HttpClient::PostAsync(const Uri& uri, const String& body, std::stop_token cancellationToken)
{
	String host = uri.GetHost();

	//auto sock = Socket::CreateTCP();
	//sock->Connect(EndPoint(IPAddress::Parse(host), 16001));

	std::string bodyCoded = body.AsCodepage(65001);
	size_t bodySize = bodyCoded.size();

	String data =
		TEXT("POST {} HTTP/1.1\r\n")
		TEXT("Host: {}:{}\r\n")
		TEXT("User-Agent: cppref/network\r\n")
		TEXT("Accept: */*\r\n")
		TEXT("Content-Type: application/json\r\n")
		TEXT("Content-Length: {}\r\n")
		TEXT("\r\n");

	std::string buffer = String::Format(data, uri.GetPath(), host, uri.GetPort(), bodySize).AsCodepage(65001);
	buffer += bodyCoded;

	//size_t send = 0;
	//while (send < buffer.length())
	//{
	//	send += co_await sock->SendAsync(buffer, cancellationToken);
	//}

	//size_t recv = 0;
	//std::string bufferToRecv;
	//bufferToRecv.resize(1024);
	//co_await sock->ReceiveAsync(bufferToRecv, cancellationToken);

	co_return TEXT("");
}