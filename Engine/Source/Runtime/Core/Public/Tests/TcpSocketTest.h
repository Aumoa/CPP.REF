// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Diagnostics/TestFixture.h"

namespace Ayla
{
	class CORE_API TcpSocketTest : public TestFixture
	{
	public:
		TcpSocketTest();
		virtual ~TcpSocketTest() noexcept override;

	protected:
		virtual std::vector<TestCase> SetUp() override;

	private:
		Task<> TestBasicOperationsServer(std::stop_token cancellationToken);
		Task<> TestBasicOperationsClient(std::stop_token cancellationToken);
		Task<> TestOperationsServerAsync(std::stop_token cancellationToken);
		Task<> TestOperationsClientAsync(std::stop_token cancellationToken);
	};
}