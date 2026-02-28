// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Tests/IPAddressTest.h"
#include "Net/IPAddress.h"
#include "Net/IPEndPoint.h"

namespace Ayla
{
	IPAddressTest::IPAddressTest()
	{
	}

	IPAddressTest::~IPAddressTest() noexcept
	{
	}

	std::vector<TestFixture::TestCase> IPAddressTest::SetUp()
	{
		return
		{
			{
				.Name = TEXT("IPv4 Parsing Tests"),
				.TestFuncs =
				{
					std::bind(&IPAddressTest::TestIPv4ParseValid, this, TEXT("0.0.0.0"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPv4ParseValid, this, TEXT("127.0.0.1"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPv4ParseValid, this, TEXT("255.255.255.255"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPv4ParseValid, this, TEXT("192.168.1.1"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPv4ParseValid, this, TEXT("10.0.0.1"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPv4ParseValid, this, TEXT("172.16.0.1"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPv4ParseValid, this, TEXT("8.8.8.8"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPv4ParseValid, this, TEXT("1.1.1.1"), std::placeholders::_1)
				}
			},
			{
				.Name = TEXT("IPv4 Invalid Parsing Tests"),
				.TestFuncs =
				{
					std::bind(&IPAddressTest::TestIPv4ParseInvalid, this, TEXT("256.1.1.1"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPv4ParseInvalid, this, TEXT("1.256.1.1"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPv4ParseInvalid, this, TEXT("1.1.256.1"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPv4ParseInvalid, this, TEXT("1.1.1.256"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPv4ParseInvalid, this, TEXT("1.1.1"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPv4ParseInvalid, this, TEXT("1.1.1.1.1"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPv4ParseInvalid, this, TEXT("1.1.1."), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPv4ParseInvalid, this, TEXT(".1.1.1"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPv4ParseInvalid, this, TEXT("1..1.1"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPv4ParseInvalid, this, TEXT("01.1.1.1"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPv4ParseInvalid, this, TEXT("1.01.1.1"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPv4ParseInvalid, this, TEXT("abc.1.1.1"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPv4ParseInvalid, this, TEXT(""), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPv4ParseInvalid, this, TEXT("   "), std::placeholders::_1)
				}
			},
			{
				.Name = TEXT("IPv6 Parsing Tests"),
				.TestFuncs =
				{
					std::bind(&IPAddressTest::TestIPv6ParseValid, this, TEXT("::"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPv6ParseValid, this, TEXT("::1"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPv6ParseValid, this, TEXT("2001:db8::1"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPv6ParseValid, this, TEXT("fe80::1"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPv6ParseValid, this, TEXT("::ffff:192.168.1.1"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPv6ParseValid, this, TEXT("2001:0db8:0000:0000:0000:0000:0000:0001"), std::placeholders::_1)
				}
			},
			{
				.Name = TEXT("IPv6 Invalid Parsing Tests"),
				.TestFuncs =
				{
					std::bind(&IPAddressTest::TestIPv6ParseInvalid, this, TEXT("invalid::address"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPv6ParseInvalid, this, TEXT("::1::2"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPv6ParseInvalid, this, TEXT("12345::1"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPv6ParseInvalid, this, TEXT("gggg::1"), std::placeholders::_1)
				}
			},
			{
				.Name = TEXT("Static Method Tests"),
				.TestFuncs =
				{
					std::bind(&IPAddressTest::TestStaticMethod, this, TEXT("Any"), TEXT("0.0.0.0"), AddressFamily::InterNetwork, std::placeholders::_1),
					std::bind(&IPAddressTest::TestStaticMethod, this, TEXT("Loopback"), TEXT("127.0.0.1"), AddressFamily::InterNetwork, std::placeholders::_1),
					std::bind(&IPAddressTest::TestStaticMethod, this, TEXT("IPv6Any"), TEXT("::"), AddressFamily::InterNetworkV6, std::placeholders::_1),
					std::bind(&IPAddressTest::TestStaticMethod, this, TEXT("IPv6Loopback"), TEXT("::1"), AddressFamily::InterNetworkV6, std::placeholders::_1)
				}
			},
			{
				.Name = TEXT("IPEndPoint Tests"),
				.TestFuncs =
				{
					std::bind(&IPAddressTest::TestIPEndPoint, this, TEXT("192.168.1.100"), (uint16)8080, TEXT("192.168.1.100:8080"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPEndPoint, this, TEXT("::1"), (uint16)9090, TEXT("[::1]:9090"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPEndPoint, this, TEXT("127.0.0.1"), (uint16)0, TEXT("127.0.0.1:0"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestIPEndPoint, this, TEXT("127.0.0.1"), (uint16)65535, TEXT("127.0.0.1:65535"), std::placeholders::_1)
				}
			},
			{
				.Name = TEXT("Round-trip Tests"),
				.TestFuncs =
				{
					std::bind(&IPAddressTest::TestRoundTrip, this, TEXT("0.0.0.0"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestRoundTrip, this, TEXT("127.0.0.1"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestRoundTrip, this, TEXT("192.168.1.1"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestRoundTrip, this, TEXT("255.255.255.255"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestRoundTrip, this, TEXT("10.0.0.1"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestRoundTrip, this, TEXT("172.16.0.1"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestRoundTrip, this, TEXT("8.8.8.8"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestRoundTrip, this, TEXT("1.1.1.1"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestRoundTrip, this, TEXT("::"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestRoundTrip, this, TEXT("::1"), std::placeholders::_1)
				}
			},
			{
				.Name = TEXT("Address Family Tests"),
				.TestFuncs =
				{
					std::bind(&IPAddressTest::TestAddressFamily, this, TEXT("192.168.1.1"), AddressFamily::InterNetwork, std::placeholders::_1),
					std::bind(&IPAddressTest::TestAddressFamily, this, TEXT("::1"), AddressFamily::InterNetworkV6, std::placeholders::_1),
					std::bind(&IPAddressTest::TestAddressFamily, this, TEXT("::ffff:192.168.1.1"), AddressFamily::InterNetworkV6, std::placeholders::_1)
				}
			},
			{
				.Name = TEXT("Equality Tests"),
				.TestFuncs =
				{
					std::bind(&IPAddressTest::TestEquality, this, TEXT("192.168.1.1"), TEXT("192.168.1.1"), TEXT("192.168.1.2"), std::placeholders::_1),
					std::bind(&IPAddressTest::TestEquality, this, TEXT("::1"), TEXT("::1"), TEXT("::"), std::placeholders::_1)
				}
			}
		};
	}

	// Unified IPv4 Valid Parsing Test
	Task<> IPAddressTest::TestIPv4ParseValid(String input, std::stop_token cancellationToken)
	{
		IPAddress result;
		Assert::True(IPAddress::TryParse(input, result));
		Assert::Equal(AddressFamily::InterNetwork, result.GetAddressFamily());
		Assert::Equal(input, result.ToString());
		co_return;
	}

	// Unified IPv4 Invalid Parsing Test
	Task<> IPAddressTest::TestIPv4ParseInvalid(String input, std::stop_token cancellationToken)
	{
		IPAddress result;
		Assert::False(IPAddress::TryParse(input, result));
		co_return;
	}

	// Unified IPv6 Valid Parsing Test
	Task<> IPAddressTest::TestIPv6ParseValid(String input, std::stop_token cancellationToken)
	{
		IPAddress result;
		Assert::True(IPAddress::TryParse(input, result));
		Assert::Equal(AddressFamily::InterNetworkV6, result.GetAddressFamily());
		co_return;
	}

	// Unified IPv6 Invalid Parsing Test
	Task<> IPAddressTest::TestIPv6ParseInvalid(String input, std::stop_token cancellationToken)
	{
		IPAddress result;
		Assert::False(IPAddress::TryParse(input, result));
		co_return;
	}

	// Unified Static Method Test
	Task<> IPAddressTest::TestStaticMethod(String methodName, String expectedString, AddressFamily expectedFamily, std::stop_token cancellationToken)
	{
		IPAddress result;
		
		if (methodName == TEXT("Any"))
		{
			result = IPAddress::Any();
		}
		else if (methodName == TEXT("Loopback"))
		{
			result = IPAddress::Loopback();
		}
		else if (methodName == TEXT("IPv6Any"))
		{
			result = IPAddress::IPv6Any();
		}
		else if (methodName == TEXT("IPv6Loopback"))
		{
			result = IPAddress::IPv6Loopback();
		}
		else
		{
			Assert::Fail(String::Format(TEXT("Unknown static method: {}"), methodName));
		}

		Assert::Equal(expectedString, result.ToString());
		Assert::Equal(expectedFamily, result.GetAddressFamily());
		co_return;
	}

	// Unified IPEndPoint Test
	Task<> IPAddressTest::TestIPEndPoint(String addressString, uint16 port, String expectedString, std::stop_token cancellationToken)
	{
		IPAddress addr = IPAddress::Parse(addressString);
		IPEndPoint endpoint(addr, port);
		Assert::Equal(expectedString, endpoint.ToString());
		co_return;
	}

	// Unified Round-trip Test
	Task<> IPAddressTest::TestRoundTrip(String addressString, std::stop_token cancellationToken)
	{
		IPAddress parsed;
		Assert::True(IPAddress::TryParse(addressString, parsed));
		
		String stringResult = parsed.ToString();
		Assert::Equal(addressString, stringResult);
		
		IPAddress roundTrip;
		Assert::True(IPAddress::TryParse(stringResult, roundTrip));
		Assert::True(parsed == roundTrip);
		co_return;
	}

	// Unified Address Family Test
	Task<> IPAddressTest::TestAddressFamily(String addressString, AddressFamily expectedFamily, std::stop_token cancellationToken)
	{
		IPAddress addr = IPAddress::Parse(addressString);
		Assert::Equal(expectedFamily, addr.GetAddressFamily());
		co_return;
	}

	// Unified Equality Test
	Task<> IPAddressTest::TestEquality(String addr1String, String addr2String, String addr3String, std::stop_token cancellationToken)
	{
		IPAddress addr1 = IPAddress::Parse(addr1String);
		IPAddress addr2 = IPAddress::Parse(addr2String);
		IPAddress addr3 = IPAddress::Parse(addr3String);

		// addr1�� addr2�� ���ƾ� ��
		Assert::True(addr1 == addr2);
		Assert::False(addr1 != addr2);
		
		// addr1�� addr3�� �޶�� ��
		Assert::False(addr1 == addr3);
		Assert::True(addr1 != addr3);
		
		co_return;
	}
}