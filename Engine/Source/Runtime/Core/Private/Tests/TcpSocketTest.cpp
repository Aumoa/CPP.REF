// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Tests/TcpSocketTest.h"
#include "Net/Sockets/Socket.h"

namespace Ayla
{
	TcpSocketTest::TcpSocketTest()
	{
	}

	TcpSocketTest::~TcpSocketTest() noexcept
	{
	}

	std::vector<TestFixture::TestCase> TcpSocketTest::SetUp()
	{
		return
		{
			{
				.Name = TEXT("TCP Socket Basic Operations - Server"),
				.TestFuncs =
				{
					std::bind(&TcpSocketTest::TestBasicOperationsServer, this, std::placeholders::_1)
				},
			},
			{
				.Name = TEXT("TCP Socket Basic Operations - Client"),
				.TestFuncs =
				{
					std::bind(&TcpSocketTest::TestBasicOperationsClient, this, std::placeholders::_1)
				},
			},
			{
				.Name = TEXT("TCP Socket Async Operations - Server"),
				.TestFuncs =
				{
					std::bind(&TcpSocketTest::TestOperationsServerAsync, this, std::placeholders::_1)
				},
			},
			{
				.Name = TEXT("TCP Socket Async Operations - Client"),
				.TestFuncs =
				{
					std::bind(&TcpSocketTest::TestOperationsClientAsync, this, std::placeholders::_1)
				},
			}
		};
	}

	Task<> TcpSocketTest::TestBasicOperationsServer(std::stop_token cancellationToken)
	{
		auto serverSocket = std::make_shared<Socket>(AddressFamily::InterNetwork, SocketType::Stream);
		Assert::NotNull(serverSocket);
		serverSocket->Bind(IPEndPoint::Any(33420));
		serverSocket->Listen();
		auto clientSocket = serverSocket->Accept();
		Assert::NotNull(clientSocket);

		std::vector<uint8> buffer(1024 * 16);
		size_t reads = 0;
		while (reads < buffer.size())
		{
			auto span = std::span<uint8>(buffer.begin() + reads, buffer.end());
			reads += clientSocket->Receive(span);
		}

		for (size_t i = 0; i < buffer.size(); ++i)
		{
			if (buffer[i] != i % 256)
			{
				Assert::Fail(TEXT("Buffer mismatch"));
				break;
			}
		}

		clientSocket->Shutdown(SocketShutdown::Send);
		while (true)
		{
			reads = clientSocket->Receive(buffer);
			if (reads == 0)
			{
				break;
			}
		}

		clientSocket->Close();
		co_return;
	}

	Task<> TcpSocketTest::TestBasicOperationsClient(std::stop_token cancellationToken)
	{
		auto clientSocket = std::make_shared<Socket>(AddressFamily::InterNetwork, SocketType::Stream);
		Assert::NotNull(clientSocket);

		clientSocket->Connect(IPEndPoint(IPAddress::Loopback(), 33421));

		std::vector<uint8> buffer(1024 * 16);
		for (size_t i = 0; i < buffer.size(); ++i)
		{
			buffer[i] = static_cast<uint8>(i % 256);
		}

		size_t sent = 0;
		while (sent < buffer.size())
		{
			auto span = std::span<const uint8>(buffer.begin() + sent, buffer.end());
			sent += clientSocket->Send(span);
		}

		clientSocket->Shutdown(SocketShutdown::Send);

		while (true)
		{
			sent = clientSocket->Receive(buffer);
			if (sent == 0)
			{
				break;
			}
		}

		clientSocket->Close();
		co_return;
	}

	Task<> TcpSocketTest::TestOperationsServerAsync(std::stop_token cancellationToken)
	{
		auto serverSocket = std::make_shared<Socket>(AddressFamily::InterNetwork, SocketType::Stream);
		Assert::NotNull(serverSocket);
		serverSocket->Bind(IPEndPoint::Any(33422));
		serverSocket->Listen();
		auto clientSocket = co_await serverSocket->AcceptAsync();
		Assert::NotNull(clientSocket);

		std::vector<uint8> buffer(1024 * 16);
		size_t reads = 0;
		while (reads < buffer.size())
		{
			auto span = std::span<uint8>(buffer.begin() + reads, buffer.end());
			reads += co_await clientSocket->ReceiveAsync(span);
		}

		for (size_t i = 0; i < buffer.size(); ++i)
		{
			if (buffer[i] != i % 256)
			{
				Assert::Fail(TEXT("Buffer mismatch"));
				break;
			}
		}

		clientSocket->Shutdown(SocketShutdown::Send);
		while (true)
		{
			reads = co_await clientSocket->ReceiveAsync(buffer);
			if (reads == 0)
			{
				break;
			}
		}

		clientSocket->Close();
		co_return;
	}

	Task<> TcpSocketTest::TestOperationsClientAsync(std::stop_token cancellationToken)
	{
		auto clientSocket = std::make_shared<Socket>(AddressFamily::InterNetwork, SocketType::Stream);
		Assert::NotNull(clientSocket);

		co_await clientSocket->ConnectAsync(IPEndPoint(IPAddress::Loopback(), 33423));

		std::vector<uint8> buffer(1024 * 16);
		for (size_t i = 0; i < buffer.size(); ++i)
		{
			buffer[i] = static_cast<uint8>(i % 256);
		}

		size_t sent = 0;
		while (sent < buffer.size())
		{
			auto span = std::span<const uint8>(buffer.begin() + sent, buffer.end());
			if (span.size_bytes() > 1024)
			{
				span = span.first(1024);
			}

			sent += co_await clientSocket->SendAsync(span);
		}

		clientSocket->Shutdown(SocketShutdown::Send);

		while (true)
		{
			sent = co_await clientSocket->ReceiveAsync(buffer);
			if (sent == 0)
			{
				break;
			}
		}

		clientSocket->Close();
		co_return;
	}
}