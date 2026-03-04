// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Diagnostics/TestFixture.h"
#include "Net/Sockets/AddressFamily.h"

namespace Ayla
{
	class CORE_API IPAddressTest : public TestFixture
	{
	public:
		IPAddressTest();
		virtual ~IPAddressTest() noexcept override;

	protected:
		virtual std::vector<TestCase> SetUp() override;

	private:
		// Unified parsing tests
		Task<> TestIPv4ParseValid(String input, std::stop_token cancellationToken);
		Task<> TestIPv4ParseInvalid(String input, std::stop_token cancellationToken);
		Task<> TestIPv6ParseValid(String input, std::stop_token cancellationToken);
		Task<> TestIPv6ParseInvalid(String input, std::stop_token cancellationToken);

		// Unified functionality tests
		Task<> TestStaticMethod(String methodName, String expectedString, AddressFamily expectedFamily, std::stop_token cancellationToken);
		Task<> TestIPEndPoint(String addressString, uint16 port, String expectedString, std::stop_token cancellationToken);
		Task<> TestRoundTrip(String addressString, std::stop_token cancellationToken);
		Task<> TestAddressFamily(String addressString, AddressFamily expectedFamily, std::stop_token cancellationToken);
		Task<> TestEquality(String addr1String, String addr2String, String addr3String, std::stop_token cancellationToken);
	};
}